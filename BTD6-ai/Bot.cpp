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

void Bot::SaveForRandomUpgrade(Game* game)
{
	int round = static_cast<int>(game->GetRoundCount());
	int remainingMoney = game->GetMoney();
	// random upgrade here
	
	int upgradePrice = 10000;
	int remaining = 500;

	Tower* randomTower = game->GetRandomTower();
	int towerIndex = 0;
	int TowerCount = game->GetTowerCount();
	while (towerIndex < TowerCount)
	{
		int purchaseRound = 0;
		int PredictiveRound = DIFFICULTY_ROUND[DIFFICULTY];

		int totalCashByRound = ((CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[PredictiveRound - 1]) - CUMULATIVE_CASH[round]) + remainingMoney;
		while (totalCashByRound < upgradePrice && PredictiveRound > round)
		{
			purchaseRound = PredictiveRound; // This is the round the bot is able to buy upgrade
			PredictiveRound--;
			totalCashByRound = ((CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[PredictiveRound - 1]) - CUMULATIVE_CASH[round]) + remainingMoney;
		}
		if (purchaseRound)
		{
			break;
		}
		towerIndex++;
		randomTower = game->GetNextRandomTower(towerIndex);
	}
	// 
	//Tower* randomTower = game->GetRandomTower();
	//short currentTowerIndex = 0;
	//bool upgradedTower = false;

	//int TowerCount = game->GetTowerCount();
	//while (!upgradedTower && currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
	//{
	//	std::shuffle(PATHS.begin(), PATHS.end(), Random::GetEngine());
	//	short currentPath = 0;
	//	while (currentPath < 3)
	//	{
	//		upgradedTower = game->UpgradeTower(randomTower, PATHS[currentPath]);
	//		if (upgradedTower)
	//		{
	//			return;
	//		}
	//		currentPath++;
	//	}
	//	currentTowerIndex++;
	//	randomTower = game->GetNextRandomTower(currentTowerIndex);
	//}
}

void Bot::run(Game* game)
{
	double previousRound = -1;
	double currentRound = game->GetRoundCount();
	while (game->GetHealth() > 0 && currentRound < DIFFICULTY_ROUND[DIFFICULTY])
	{
		if (currentRound > previousRound)
		{
			game->GetMoney();
			std::cout << "Next Round: " << currentRound << "\n";
			previousRound = currentRound;
			int chance = Random::getValue(1, 100); // Weakness: Can only do one action at a time each round (Max is 6 actions?
			// Make bot able to save money to buy expensive tower
			if (chance <= Buy_Chance)
			{
				BuyRandomTower(game);
			}
			else if (chance <= Upgrade_Chance)
			{
				UpgradeRandomTower(game);
			}
			std::cout << "-Start Next Round-\n";
			game->StartNextRound();
		}
		currentRound = game->GetRoundCount();
	}
}
