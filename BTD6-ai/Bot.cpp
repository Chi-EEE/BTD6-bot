#include "Bot.h"

#include <vector>

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
	ClientPosition = game->GetPlayPosition();
	ClientSize = game->GetPlaySize();
}

void Bot::run(Game* game)
{
	double previousRound = 0;
	int currentRound = static_cast<int>(game->GetRoundCount());

	game->GetMoney();
	BuyRandomTower(game);
	game->StartNextRound();
	// Splkit buy and upgrade into two pieces and make sasve or immediate options 50/50
	while (game->GetHealth() > 0 && currentRound < DIFFICULTY_ROUND[DIFFICULTY])
	{
		if (currentRound > previousRound)
		{
			std::cout << "Next Round: " << currentRound << "\n";
			previousRound = currentRound;
			for (int i = 0; i < buildActions.size(); i++)
			{
				if (buildActions[i].isReady(currentRound))
				{
					game->GetMoney();
					TowerName towerName = buildActions[i].getTowerName();

					int buildAttempts = 0;
					bool built = false;
					while (buildAttempts < MaximumBuildAttempts)
					{
						built = game->PlaceTower(towerName, GetRandomPosition());
						if (built)
						{
							break;
						}
						buildAttempts++;
					}
					buildActions.erase(buildActions.begin() + i);
					savingForPurchase = false;
				}
			}
			for (int i = 0; i < upgradeActions.size(); i++)
			{
				if (upgradeActions[i].isReady(currentRound)) // Problem, attempts to upgrade tower when not enough money by its expected round (due to leaking bloons)
				{ // Also cannot upgrade to third tier (could of upgraded early)
					game->GetMoney();
					std::cout << "Bought saved upgrade\n";
					Tower* tower = upgradeActions[i].getTower();

					game->UpgradeTower(tower, upgradeActions[i].getPath());
					upgradeActions.erase(upgradeActions.begin() + i);
					savingForPurchase = false;
				}
			}
			if (!savingForPurchase)
			{
				int optionChance = Random::getValue(1, 100); // Weakness: Can only do one action at a time each round (Max is 6 actions?
				int buyOrSaveChance = Random::getValue(1, 100);

				if (optionChance <= Buy_Chance)
				{
					if (buyOrSaveChance <= 60) // Buy
					{
						std::cout << "Buying Random\n";
						BuyRandomTower(game);
					}
					else // Save 40%
					{
						std::cout << "Save Buy Random\n";
						SaveForRandomTower(game);
					}
				}
				else // Upgrading
				{
					if (buyOrSaveChance <= 60) // Buy
					{
						std::cout << "Buying Random Upgrade\n";
						UpgradeRandomTower(game);
					}
					else // Save 40%
					{
						std::cout << "Save Buy Random Upgrade\n";
						SaveForRandomUpgrade(game);
					}
				}
			}
			std::cout << "-Start Next Round-\n";
			game->StartNextRound();
		}
		currentRound = static_cast<int>(game->GetRoundCount());
	}
}

bool Bot::BuyRandomTower(Game* game)
{
	game->GetMoney();

	std::shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end(), Random::GetEngine());
	short currentTower = 0;
	while (currentTower < ALLOWED_TOWERS.size())
	{
		std::cout << "Attempting to build tower id: " << static_cast<int>(ALLOWED_TOWERS[currentTower]) << "\n";
		if (game->CanBuildTower(ALLOWED_TOWERS[currentTower]))
		{
			int buildAttempts = 0;
			bool built = false;

			// Keep on building until reached MaximumBuildAttempts
			while (buildAttempts < MaximumBuildAttempts)
			{
				built = game->PlaceTower(ALLOWED_TOWERS[currentTower], GetRandomPosition());
				if (built)
				{
					return true;
				}
				buildAttempts++;
			}
		}
		currentTower++;
	}
	return false;
}

bool Bot::SaveForRandomTower(Game* game)
{
	double health = game->GetHealth();
	double money = game->GetMoney();
	int round = static_cast<int>(game->GetRoundCount());

	double addedMoney = money - (health - 1);
	int towerIndex = 0;

	std::shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end(), Random::GetEngine());
	while (towerIndex < ALLOWED_TOWERS.size())
	{
		int towerPrice = game->MultiplyDefaultPrice(TOWER_BASE_COST[static_cast<int>(ALLOWED_TOWERS[towerIndex])]);
		if (money < towerPrice)
		{
			int PredictiveRound = min(round + 3, DIFFICULTY_ROUND[DIFFICULTY] - 1);
			int purchaseRound = 0;

			double totalCashByRound = -1;

			while (PredictiveRound > round)
			{
				totalCashByRound = (CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[round]) + addedMoney;
				if (totalCashByRound < towerPrice)
				{
					if (purchaseRound)
					{
						std::cout << "Saving Cash for towerIndex: " << static_cast<int>(ALLOWED_TOWERS[towerIndex]) <<
							" costing " << towerPrice <<
							" until round (displayed)" << purchaseRound + 1 <<
							" with " << totalCashByRound << " money." << "\n";
						BuildAction buildAction = BuildAction{ round, ALLOWED_TOWERS[towerIndex] };
						buildActions.push_back(buildAction);
						savingForPurchase = true;
						return true;
					}
					break;
				}
				purchaseRound = PredictiveRound;
				PredictiveRound--;
			}
		}
		towerIndex++;
	}
	return false;
}

bool Bot::UpgradeRandomTower(Game* game)
{
	game->GetMoney();

	int TowerCount = game->GetTowerCount();
	Tower* randomTower = nullptr;

	bool upgradedTower = false;
	short currentTowerIndex = 0;

	game->RandomizeTowers();
	while (currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
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
				return true;
			}
			currentPath++;
		}
	}
	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="tower"></param>
/// <param name="health"></param>
/// <param name="remainingMoney"></param>
/// <param name="round"></param>
/// <param name="upgradePrice"></param>
/// <param name="path"></param>
/// <returns></returns>
bool Bot::GetUpgradeRound(Tower* tower, double health, double remainingMoney, int round, double upgradePrice, short path)
{
	int PredictiveRound = min(round + 10, DIFFICULTY_ROUND[DIFFICULTY] - 1);
	int purchaseRound = 0;

	double addedMoney = remainingMoney - (health - 1);
	double totalCashByRound = -1;
	
	std::cout << "Predictive Round: " << PredictiveRound << "\n";
	while (PredictiveRound > round)
	{
		totalCashByRound = (CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[round]) + addedMoney;
		std::cout << "Total cash by round << " << PredictiveRound << ": " << totalCashByRound << ", Upgrade price: " << upgradePrice << "\n";
		if (totalCashByRound < upgradePrice)
		{
			std::cout << "-------Total cash by round << " << PredictiveRound << ": " << totalCashByRound << "\n";
			if (purchaseRound)
			{
				std::cout << "Saving Cash for " << tower->GetId() << " at path " << path + 1 << " costing " << upgradePrice << " until round (displayed)" << purchaseRound + 1 << " with " << totalCashByRound << " money." << "\n";
				UpgradeAction newAction = UpgradeAction{ purchaseRound, tower, path };
				upgradeActions.push_back(newAction);
				savingForPurchase = true;
				return true;
			}
			break;
		}
		purchaseRound = PredictiveRound;
		PredictiveRound--;
	}
	return false;
}

bool Bot::SaveForRandomUpgrade(Game* game)
{
	int round = static_cast<int>(game->GetRoundCount());
	double money = game->GetMoney();
	double health = game->GetHealth();
	int TowerCount = game->GetTowerCount();

	Tower* randomTower = nullptr;

	short currentTowerIndex = 0;
	game->RandomizeTowers();
	while (currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
	{
		randomTower = game->GetRandomTower(currentTowerIndex);

		short currentPath = 0;
		const int towerId = randomTower->GetId();

		std::shuffle(PATHS.begin(), PATHS.end(), Random::GetEngine());
		while (currentPath < 3)
		{
			const int path = PATHS[currentPath];

			short LatestPath = randomTower->GetUpgradePath()[path];
			if (randomTower->IsValidPath(path))
			{
				bool gotUpgradeRound = GetUpgradeRound(randomTower, health, money, round, game->MultiplyDefaultPrice(TOWER_UPGRADE[towerId][path][LatestPath]), path);
				if (gotUpgradeRound)
				{
					return true;
				}
			}
			currentPath++;
		}
		currentTowerIndex++;
		std::cout << "Next tower: " << currentTowerIndex << "\n";
	}
	return false;
}