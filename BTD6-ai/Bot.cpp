#include "Bot.h"

#include <vector>
#include <iomanip>
#include <fstream>

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
					if (game->CanBuildTower(towerName))
					{
						int buildAttempts = 0;
						Tower* tower = nullptr;

						// Keep on building until reached MaximumBuildAttempts
						while (buildAttempts < MaximumBuildAttempts)
						{
							tower = game->PlaceTower(towerName, GetRandomPosition());
							if (tower)
							{
								std::cout << "[BUILD][PURCHASED]: " << static_cast<int>(towerName) << "\n";

								currentBuildAction->setPosition(tower->GetPosition());
								previousActions.push_back(new BuildAction{ currentBuildAction });
								break;
							}
							buildAttempts++;
						}
						delete actions.front();
						actions.pop();
					}
				}
				break;
				case ActionType::Upgrade:
				{
					UpgradeAction* currentUpgradeAction = (UpgradeAction*)actions.front();
					Tower* currentTower = game->GetTower(currentUpgradeAction->getTowerIndex());

					if (game->UpgradeTower(currentTower, currentUpgradeAction->getPath()))
					{
						std::cout << "[UPGRADE][PURCHASED]: " << static_cast<int>(currentTower->getTowerName()) << "\n";
						previousActions.push_back(actions.front());
						actions.pop();
					}
				}
				break;
				}
			}
			else
			{
				int optionChance = Random::getValue(1, 100); // Weakness: Can only do one action at a time each round (Max is 6 actions?
				int buyOrSaveChance = Random::getValue(1, 100);

				if (optionChance <= Buy_Chance)
				{
					std::cout << "[BUILD]";
					if (buyOrSaveChance <= 60) // Buy
					{
						std::cout << "[BUYING]";
						BuyRandomTower(game);
					}
					else // Save 40%
					{
						//std::cout << "Save Buy Random\n"
						std::cout << "[SAVE]";;
						if (!SaveForRandomTower(game))
						{
							std::cout << "[BUYING]";
							//std::cout << "nvm\n";
							BuyRandomTower(game);
						}
					}
				}
				else // Upgrading
				{
					std::cout << "[UPGRADE]";
					if (buyOrSaveChance <= 60) // Buy
					{
						std::cout << "[BUYING]";
						UpgradeRandomTower(game);
					}
					else // Save 40%
					{
						std::cout << "[SAVE]";;
						SaveForRandomUpgrade(game);
					}
				}
			}
			std::cout << "-[NEXT ROUND : " << currentRound + 1 << "]-\n";
			game->StartNextRound();
		}
		currentRound = static_cast<int>(game->GetRoundCount());
	}
	Save();
}

bool Bot::BuyHero(Game* game)
{
	game->GetMoney();
	double round = game->GetRoundCount();

	if (game->CanBuildTower(TowerName::Hero))
	{
		int buildAttempts = 0;
		Tower* tower = false;

		// Keep on building until reached MaximumBuildAttempts
		while (buildAttempts < MaximumBuildAttempts)
		{
			tower = game->PlaceTower(TowerName::Hero, GetRandomPosition());
			if (tower)
			{
				std::cout << " Tower ID:[" << static_cast<int>(TowerName::Hero) << "]\n";
				previousActions.push_back(new BuildAction{ static_cast<int>(round), TowerName::Hero, tower->GetPosition() });
				return true;
			}
			buildAttempts++;
		}
	}
	std::cout << "[NVM]\n";
	return false;
}

bool Bot::BuyRandomTower(Game* game)
{
	game->GetMoney();

	double round = game->GetRoundCount();
	std::shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end(), Random::GetEngine());
	short currentTowerCount = 0;
	while (currentTowerCount < ALLOWED_TOWERS.size())
	{
		//std::cout << "Attempting to build tower id: " << static_cast<int>(ALLOWED_TOWERS[currentTower]) << "\n";
		TowerName towerName = ALLOWED_TOWERS[currentTowerCount];
		if (game->CanBuildTower(towerName))
		{
			int buildAttempts = 0;
			Tower* tower = nullptr;

			// Keep on building until reached MaximumBuildAttempts
			while (buildAttempts < MaximumBuildAttempts)
			{
				tower = game->PlaceTower(towerName, GetRandomPosition());
				if (tower)
				{
					std::cout << " Tower ID:[" << static_cast<int>(ALLOWED_TOWERS[currentTowerCount]) << "]\n";

					previousActions.push_back(new BuildAction{ static_cast<int>(round), towerName, tower->GetPosition() });
					return true;
				}
				buildAttempts++;
			}
		}
		currentTowerCount++;
	}
	std::cout << "[NVM]\n";
	return false;
}

bool Bot::SaveForRandomTower(Game* game)
{
	double health = game->GetHealth();
	double money = game->GetMoney();
	int round = static_cast<int>(game->GetRoundCount());

	double addedMoney = money - (health - 1);
	int currentTowerCount = 0;

	std::shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end(), Random::GetEngine());
	while (currentTowerCount < ALLOWED_TOWERS.size())
	{
		int towerPrice = game->MultiplyDefaultPrice(TOWER_BASE_COST[static_cast<int>(ALLOWED_TOWERS[currentTowerCount])]);
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
						std::cout << " Tower ID:[" << static_cast<int>(ALLOWED_TOWERS[currentTowerCount]) << "] | Price:[" << towerPrice << "] | Round:[" << purchaseRound + 1 << "]\n";
						actions.push(new BuildAction{ purchaseRound + 1, ALLOWED_TOWERS[currentTowerCount] });
						return true;
					}
					break;
				}
				purchaseRound = PredictiveRound;
				PredictiveRound--;
			}
		}
		currentTowerCount++;
	}
	return false;
}

bool Bot::UpgradeRandomTower(Game* game)
{
	game->GetMoney();
	double round = game->GetRoundCount();
	int TowerCount = game->GetTowerCount();

	short towerIndex = 0;
	Tower* randomTower = nullptr;

	bool upgradedTower = false;
	short currentTowerIndex = 0;

	game->RandomizeTowers();
	while (currentTowerIndex < TowerCount) // Loop through all towers to see if able to be upgraded
	{
		towerIndex = game->GetRandomTowerIndex(currentTowerIndex);
		randomTower = game->GetTower(towerIndex);

		std::shuffle(PATHS.begin(), PATHS.end(), Random::GetEngine());
		short currentPath = 0;
		while (currentPath < 3)
		{
			short path = PATHS[currentPath];
			upgradedTower = game->UpgradeTower(randomTower, path);
			if (upgradedTower)
			{
				std::cout << " Tower ID:[" << randomTower->GetId() << "]\n";
				previousActions.push_back(new UpgradeAction{ static_cast<int>(round), randomTower->GetId(), path });
				return true;
			}
			currentPath++;
		}
		currentTowerIndex++;
	}
	return false;
}

bool Bot::GetUpgradeRound(Game* game, Tower* tower, short towerIndex, double upgradePrice, short path)
{
	double money = game->GetMoney();
	double health = game->GetHealth();
	int round = static_cast<int>(game->GetRoundCount());

	int PredictiveRound = min(round + 10, DIFFICULTY_ROUND[DIFFICULTY] - 1);
	int purchaseRound = 0;

	double addedMoney = money - (health - 1);
	double totalCashByRound = -1;
	
	//std::cout << "Predictive Round: " << PredictiveRound << "\n";
	while (PredictiveRound > round)
	{
		totalCashByRound = (CUMULATIVE_CASH[PredictiveRound] - CUMULATIVE_CASH[round]) + addedMoney;
		//std::cout << "Total cash by round << " << PredictiveRound << ": " << totalCashByRound << ", Upgrade price: " << upgradePrice << "\n";
		if (totalCashByRound < upgradePrice)
		{
			//std::cout << "-------Total cash by round << " << PredictiveRound << ": " << totalCashByRound << "\n";
			if (purchaseRound)
			{
				std::cout << " Tower ID:[" << tower->GetId() << "] | Price:[" << upgradePrice << "] | Round:[" << purchaseRound + 1 << "]\n";
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
	int TotalTowerCount = game->GetTowerCount();

	Tower* tower = nullptr;
	short towerIndex = 0;

	short currentTowerCount = 0;
	game->RandomizeTowers();
	while (currentTowerCount < TotalTowerCount) // Loop through all towers to see if able to be upgraded
	{
		towerIndex = game->GetRandomTowerIndex(currentTowerCount);
		tower = game->GetTower(towerIndex);

		short currentPath = 0;
		const int towerId = tower->GetId();

		std::shuffle(PATHS.begin(), PATHS.end(), Random::GetEngine());
		while (currentPath < 3)
		{
			const int path = PATHS[currentPath];

			short LatestPath = tower->GetUpgradePath()[path];

			if (game->UpgradeTower(tower, path)) // Could try to remove double check valid path
			{
				return true;
			}
			if (tower->IsValidPath(path))
			{
				bool gotUpgradeRound = GetUpgradeRound(game, tower, towerIndex, game->MultiplyDefaultPrice(TOWER_UPGRADE[towerId][path][LatestPath]), path);
				if (gotUpgradeRound)
				{
					return true;
				}
			}
			currentPath++;
		}
		currentTowerCount++;
		//std::cout << "Next tower: " << towerCount << "\n";
	}
	return false;
}

void Bot::Save()
{
	json saveJson;
	//saveJson["Towers"] = json::parse(previousActions);
	//std::cout << saveJson.dump();

	json actionJson;
	for (int i = 0; i < previousActions.size(); i++)
	{
		actionJson += previousActions[i]->toJson();
		delete previousActions[i];
		previousActions.erase(previousActions.begin() + i);
	}
	saveJson["Actions"] = actionJson;

	std::ofstream o("t.json");
	o << std::setw(4) << saveJson << std::endl;
}
