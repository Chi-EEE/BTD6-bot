#include "Bot.h"
#include "Globals.h"
#include "Random.h"
#include "Tower.h"

Vector2 Bot::GetRandomPosition()
{
	int x_axis = ClientPosition.X + Random::getValue(1, ClientSize.X);
	int y_axis = ClientPosition.Y + Random::getValue(1, ClientSize.Y);
	return Vector2{ x_axis, y_axis };
}

Bot::Bot(Game* game)
{
	ClientPosition = game->GetClientPosition();
	ClientSize = game->GetClientSize();
}

void Bot::BuyRandomTower(Game* game)
{
	std::shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end(), Random::GetEngine());
	short currentTower = 0;
	while (currentTower < ALLOWED_TOWERS.size())
	{
		if (game->CanBuildTower(ALLOWED_TOWERS[currentTower]))
		{
			int buildAttempts = 0;
			bool built = game->PlaceTower(ALLOWED_TOWERS[currentTower], GetRandomPosition());

			// Keep on building until reached MaximumBuildAttempts
			while (!built && buildAttempts < MaximumBuildAttempts)
			{
				built = game->PlaceTower(ALLOWED_TOWERS[currentTower], GetRandomPosition());
				buildAttempts++;
			}
			if (built)
			{
				break;
			}
		}
		currentTower++;
	}
}

void Bot::UpgradeRandomTower(Game* game)
{
	Tower* randomTower = game->GetRandomTower();
	short currentTowerIndex = 0;
	bool upgradedTower = false;

	int TowerCount = game->GetTowerCount();
	while (!upgradedTower && currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
	{
		std::shuffle(PATHS.begin(), PATHS.end(), Random::GetEngine());
		short currentPath = 0;
		while (currentPath < 3)
		{
			upgradedTower = game->UpgradeTower(randomTower, PATHS[currentPath]);
			if (upgradedTower)
			{
				return;
			}
			currentPath++;
		}
		currentTowerIndex++;
		randomTower = game->GetNextRandomTower(currentTowerIndex);
	}
}

/// <summary>
/// Part of saving money for upgrade
/// </summary>
/// <param name="tower"></param>
/// <param name="remainingMoney"></param>
/// <param name="round"></param>
/// <param name="upgradePrice"></param>
/// <param name="path"></param>
/// <returns></returns>
bool Bot::GetUpgradeRound(Tower* tower, double remainingMoney, int round, double upgradePrice, short path)
{
	int purchaseRound = 0;
	int PredictiveRound = DIFFICULTY_ROUND[DIFFICULTY];

	int totalCashByRound = ((CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[round]) - CUMULATIVE_CASH[round - 1]) + remainingMoney;
	while (totalCashByRound < upgradePrice && PredictiveRound > round)
	{
		purchaseRound = PredictiveRound; // This is the round the bot is able to buy upgrade
		PredictiveRound--;
		totalCashByRound = ((CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[round]) - CUMULATIVE_CASH[round - 1]) + remainingMoney;
	}
	if (purchaseRound)
	{
		std::cout << "Saving cash for " << tower->GetId() << " at path " << path << " until round " << purchaseRound << "\n";
		UpgradeAction newAction = UpgradeAction{ purchaseRound, tower, path };
		upgradeActions.push_back(newAction);
		return true;
	}
	return false;
}

void Bot::SaveForRandomUpgrade(Game* game)
{
	int round = static_cast<int>(game->GetRoundCount());
	int remainingMoney = game->GetMoney();
	 
	Tower* randomTower = game->GetRandomTower();
	short currentTowerIndex = 0;

	int TowerCount = game->GetTowerCount();
	while (currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
	{
		std::shuffle(PATHS.begin(), PATHS.end(), Random::GetEngine());
		short currentPath = 0;
		const int towerId = randomTower->GetId();

		while (currentPath < 3)
		{
			const int path = PATHS[currentPath];

			short LatestPath = randomTower->GetUpgradePath()[path];
			if (randomTower->IsValidPath(path))
			{
				bool gotUpgradeRound = GetUpgradeRound(randomTower, remainingMoney, round, game->MultiplyDefaultPrice(TOWER_UPGRADE[towerId][path][LatestPath]), path);
				if (gotUpgradeRound)
				{
					savingForTower = true;
					return; // Done upgrade round
				}
			}
			currentPath++;
		}
		currentTowerIndex++;
		randomTower = game->GetNextRandomTower(currentTowerIndex);
	}
}

void Bot::run(Game* game)
{
	double previousRound = 0;
	double currentRound = game->GetRoundCount();

	game->GetMoney();
	BuyRandomTower(game);
	game->StartNextRound();

	while (game->GetHealth() > 0 && currentRound < DIFFICULTY_ROUND[DIFFICULTY])
	{
		if (currentRound > previousRound)
		{
			game->GetMoney();
			std::cout << "Next Round: " << currentRound << "\n";
			previousRound = currentRound;
			int chance = Random::getValue(1, 100); // Weakness: Can only do one action at a time each round (Max is 6 actions?
			// Make bot able to save money to buy expensive tower
			for (UpgradeAction upgradeAction : upgradeActions)
			{
				if (upgradeAction.isReady(currentRound))
				{
					std::cout << "Bought saved upgrade\n";
					Tower* tower = upgradeAction.getTower();
					game->UpgradeTower(tower, upgradeAction.getPath());
					savingForTower = false;
				}
			}
			if (!savingForTower)
			{
				if (chance <= Buy_Chance)
				{
					BuyRandomTower(game);
				}
				else if (chance <= Upgrade_Chance)
				{
					UpgradeRandomTower(game);
				}
				else
				{
					SaveForRandomUpgrade(game);
				}
			}
			std::cout << "-Start Next Round-\n";
			game->StartNextRound();
		}
		currentRound = game->GetRoundCount();
	}
}
