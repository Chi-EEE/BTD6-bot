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
	game->RandomizeTowers();
	bool upgradedTower = false;

	short currentTowerIndex = 0;
	int TowerCount = game->GetTowerCount();
	Tower* randomTower = nullptr;

	while (!upgradedTower && currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
	{
		randomTower = game->GetRandomTower(currentTowerIndex);
		currentTowerIndex++;
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
	int PredictiveRound = DIFFICULTY_ROUND[DIFFICULTY] - 1;

	int totalCashByRound = (CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[round]) + remainingMoney;
	//std::cout << "Save Round: " << round << " with " << remainingMoney << "\n";
	//std::cout << "Cash: " << totalCashByRound << " by round: " << PredictiveRound << ". Upgrade Price:" << upgradePrice << "\n";
	while (totalCashByRound > upgradePrice && PredictiveRound > round)
	{
		purchaseRound = PredictiveRound; // This is the round the bot is able to buy upgrade
		PredictiveRound--;
		totalCashByRound = (CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[round]) + remainingMoney;
		//std::cout << "Cash: " << totalCashByRound << " by round: " << PredictiveRound << ". Upgrade Price:" << upgradePrice << "\n";
	}

	// Not saving for 10 rounds straight
	if (round - purchaseRound > 10)
	{
		return false;
	}
	if (purchaseRound)
	{
		std::cout << "Saving Cash for " << tower->GetId() << " at path " << path + 1<< " costing " << upgradePrice << " until round (displayed)" << purchaseRound + 1 << " with " << totalCashByRound << " money." << "\n";
		UpgradeAction newAction = UpgradeAction{ purchaseRound, tower, path };
		upgradeActions.push_back(newAction);
		return true;
	}
	return false;
}

void Bot::SaveForRandomUpgrade(Game* game)
{
	game->RandomizeTowers();

	int round = static_cast<int>(game->GetRoundCount());
	double remainingMoney = game->GetMoney();
	 
	short currentTowerIndex = 0;
	int TowerCount = game->GetTowerCount();
	Tower* randomTower = nullptr;

	while (currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
	{
		randomTower = game->GetRandomTower(currentTowerIndex);
		currentTowerIndex++;

		std::shuffle(PATHS.begin(), PATHS.end(), Random::GetEngine());
		short currentPath = 0;
		const int towerId = randomTower->GetId();

		while (currentPath < 3)
		{
			const int path = PATHS[currentPath];

			short LatestPath = randomTower->GetUpgradePath()[path];
			if (randomTower->IsValidPath(path))
			{
				if (game->UpgradeTower(randomTower, PATHS[currentPath])) // Try to upgrade tower instead of saving for lower value than money
				{
					return;
				}
				bool gotUpgradeRound = GetUpgradeRound(randomTower, remainingMoney, round, game->MultiplyDefaultPrice(TOWER_UPGRADE[towerId][path][LatestPath]), path);
				if (gotUpgradeRound)
				{
					savingForTower = true;
					return; // Done upgrade round
				}
			}
			currentPath++;
		}
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
			std::cout << "Next Round: " << currentRound << "\n";
			previousRound = currentRound;
			int chance = Random::getValue(1, 100); // Weakness: Can only do one action at a time each round (Max is 6 actions?
			// Make bot able to save money to buy expensive tower
			for (int i = 0; i < upgradeActions.size(); i++)
			{
				if (upgradeActions[i].isReady(currentRound)) // Problem, attempts to upgrade tower when not enough money by its expected round (due to leaking bloons)
				{ // Also cannot upgrade to third tier (could of upgraded early)
					game->GetMoney();
					std::cout << "Bought saved upgrade\n";
					Tower* tower = upgradeActions[i].getTower();
					game->UpgradeTower(tower, upgradeActions[i].getPath());
					upgradeActions.erase(upgradeActions.begin() + i);
					savingForTower = false;
				}
			}
			if (!savingForTower)
			{
				game->GetMoney();
				if (chance <= Buy_Chance)
				{
					std::cout << "Building Tower..\n";
					BuyRandomTower(game);
				}
				else if (chance <= Upgrade_Chance)
				{
					std::cout << "Upgrading Tower..\n";
					UpgradeRandomTower(game);
				}
				else
				{
					std::cout << "Saving for random upgrade Tower..\n";
					SaveForRandomUpgrade(game);
				}
			}
			std::cout << "-Start Next Round-\n";
			game->StartNextRound();
		}
		currentRound = game->GetRoundCount();
	}
}
