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
	BuyHero(game);
	game->StartNextRound();
	// Splkit buy and upgrade into two pieces and make sasve or immediate options 50/50
	while (game->GetHealth() > 0 && currentRound < DIFFICULTY_ROUND[DIFFICULTY])
	{
		if (currentRound > previousRound)
		{
			std::cout << "Next Round: " << currentRound << "\n";
			previousRound = currentRound;
			game->GetMoney();
			if (!actions.empty())
			{
				switch (actions.front()->getType())
				{
				case ActionType::Build:
				{
					BuildAction* currentBuildAction = (BuildAction*)actions.front();
					TowerName towerName = currentBuildAction->getTowerName();
					std::cout << "builddd\n";
					if (game->CanBuildTower(towerName))
					{
						int buildAttempts = 0;
						bool built = false;

						// Keep on building until reached MaximumBuildAttempts
						while (buildAttempts < MaximumBuildAttempts)
						{
							built = game->PlaceTower(towerName, GetRandomPosition());
							if (built)
							{
								std::cout << "Bought saved tower\n";
								break;
							}
							buildAttempts++;
						}
						// Remove action because can't build tower
						//previousActions.push_back(actions.front());
						delete actions.front();
						actions.pop();
					}
				}
				break;
				case ActionType::Upgrade:
				{
					std::cout << "upppp\n";
					UpgradeAction* currentUpgradeAction = (UpgradeAction*)actions.front();
					Tower* currentTower = &(game->getTowers()[currentUpgradeAction->getTowerIndex()]);

					if (game->UpgradeTower(currentTower, currentUpgradeAction->getPath()))
					{
						std::cout << "Bought saved upgrade\n";
						actions.pop();
					}
				}
				break;
				default:
					std::cout << static_cast<int>(actions.front()->getType()) << ": Action int\n";
				}
			}
			else
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
						if (!SaveForRandomTower(game))
						{
							std::cout << "nvm\n";
							BuyRandomTower(game);
						}
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

bool Bot::BuyHero(Game* game)
{
	game->GetMoney();

	if (game->CanBuildTower(TowerName::Hero))
	{
		int buildAttempts = 0;
		bool built = false;

		// Keep on building until reached MaximumBuildAttempts
		while (buildAttempts < MaximumBuildAttempts)
		{
			built = game->PlaceTower(TowerName::Hero, GetRandomPosition());
			if (built)
			{
				return true;
			}
			buildAttempts++;
		}
	}
	return false;
}

bool Bot::BuyRandomTower(Game* game)
{
	game->GetMoney();

	std::shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end(), Random::GetEngine());
	short currentTower = 0;
	while (currentTower < ALLOWED_TOWERS.size())
	{
		std::cout << "Attempting to build tower id: " << static_cast<int>(ALLOWED_TOWERS[currentTower]) << "\n";
		TowerName towerName = ALLOWED_TOWERS[currentTower];
		if (game->CanBuildTower(towerName))
		{
			int buildAttempts = 0;
			bool built = false;

			// Keep on building until reached MaximumBuildAttempts
			while (buildAttempts < MaximumBuildAttempts)
			{
				built = game->PlaceTower(towerName, GetRandomPosition());
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
						actions.push(new BuildAction{ round, ALLOWED_TOWERS[towerIndex] });
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
			short path = PATHS[currentPath];
			upgradedTower = game->UpgradeTower(randomTower, path);
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
bool Bot::GetUpgradeRound(Game* game, short towerIndex, double upgradePrice, short path)
{
	double money = game->GetMoney();
	double health = game->GetHealth();
	int round = static_cast<int>(game->GetRoundCount());

	int PredictiveRound = min(round + 10, DIFFICULTY_ROUND[DIFFICULTY] - 1);
	int purchaseRound = 0;

	double addedMoney = money - (health - 1);
	double totalCashByRound = -1;
	
	std::cout << "Predictive Round: " << PredictiveRound << "\n";
	while (PredictiveRound > round)
	{
		totalCashByRound = (CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[round]) + addedMoney;
		//std::cout << "Total cash by round << " << PredictiveRound << ": " << totalCashByRound << ", Upgrade price: " << upgradePrice << "\n";
		if (totalCashByRound < upgradePrice)
		{
			//std::cout << "-------Total cash by round << " << PredictiveRound << ": " << totalCashByRound << "\n";
			if (purchaseRound)
			{
				//std::cout << "Saving Cash for " << tower->GetId() << " at path " << path + 1 << " costing " << upgradePrice << " until round (displayed)" << purchaseRound + 1 << " with " << totalCashByRound << " money." << "\n";
				actions.push(new UpgradeAction{ purchaseRound, towerIndex, path });
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
	short randomTowerIndex = 0;

	short currentTowerIndex = 0;
	game->RandomizeTowers();
	while (currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
	{
		randomTowerIndex = game->GetRandomTowerIndex(currentTowerIndex);
		randomTower = game->GetRandomTower(currentTowerIndex);

		short currentPath = 0;
		const int towerId = randomTower->GetId();

		std::shuffle(PATHS.begin(), PATHS.end(), Random::GetEngine());
		while (currentPath < 3)
		{
			const int path = PATHS[currentPath];

			short LatestPath = randomTower->GetUpgradePath()[path];

			if (game->UpgradeTower(randomTower, path)) // Could try to remove double check valid path
			{
				return true;
			}
			if (randomTower->IsValidPath(path))
			{
				bool gotUpgradeRound = GetUpgradeRound(game, randomTowerIndex, game->MultiplyDefaultPrice(TOWER_UPGRADE[towerId][path][LatestPath]), path);
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

void Bot::Save()
{
	/*json saveJson;
	saveJson["Towers"] = json::parse(previousActions);*/
}
