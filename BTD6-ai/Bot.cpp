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

void Bot::run(Game* game)
{
	int previousRound = 0;
	int currentRound = game->GetRoundCount();
	while (game->GetHealth() > 0 && currentRound < DIFFICULTY_ROUND[DIFFICULTY])
	{
		if (currentRound > previousRound)
		{
			previousRound = currentRound;
			int chance = Random::getValue(1, 100);
			if (chance <= Buy_Chance)
			{
				std::random_shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end());
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
					}
					currentTower++;
				}
			}
		}
		currentRound = game->GetRoundCount();
	}

	//while (health > 0 && nowRound < DIFFICULTY_ROUND[difficulty])
	//{
	//	ReadProcessMemory(handle, roundAddress, &nowRound, sizeof(nowRound), NULL);
	//	if (nowRound > previousRound) // Runs the following when the round finishes
	//	{
	//		ReadProcessMemory(handle, moneyAddress, &money, sizeof(money), NULL);

	//		int chance = Random::getValue(1, 100);
	//		// Don't need to go into big loop because the round didn't start
	//		if (chance <= Buy_Chance)
	//		{
	//			std::random_shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end());

	//			int currentBuildAttempts = 0;
	//			short towerIndex = 0;
	//			std::cout << "Placing Tower\n";
	//			while (towerIndex < ALLOWED_TOWERS.size())
	//			{
	//				if (money > getDifficultyPrice(difficulty, TOWER_BASE_COST[static_cast<int>(ALLOWED_TOWERS[towerIndex])]))
	//				{
	//					/// <summary>
	//					/// Gets a random position and tries to place tower in that position. 
	//					/// </summary>
	//					TowerName towerName;
	//					Vector2 position;
	//					bool builtTower = false;

	//					while (towerCount <= Tower::getTowerCount() && !(currentBuildAttempts >= Build_Attempts))
	//					{
	//						{ // Place Tower
	//							position = getRandomPosition();

	//							towerName = ALLOWED_TOWERS[towerIndex];
	//							INPUT input = Keyboard::keyPress(TOWER_SCAN_CODE[static_cast<int>(towerName)]);

	//							Mouse::setPosition(position);
	//							Mouse::leftMouseDown();

	//							Clock::wait(.1f);
	//							Keyboard::keyRelease(input);
	//							Mouse::leftMouseUp();
	//							Mouse::setPosition(offPosition);
	//						}

	//						ReadProcessMemory(handle, towerCountAddress, &towerCount, sizeof(towerCount), NULL);
	//						currentBuildAttempts++;
	//						Clock::wait(1.f);
	//						builtTower = true;
	//					} // Tower Count updates after it first places, example: place then another place and position is at the second place
	//					if (builtTower && !(currentBuildAttempts >= Build_Attempts))
	//					{
	//						std::cout << static_cast<int>(towerName) << "\n";
	//						Tower newTower = Tower{ towerName, position };
	//						game.AddTower(newTower);
	//					}
	//					std::cout << "Done Building\n";
	//					break;
	//				}
	//				else
	//					towerIndex++;
	//			}
	//		}
	//		else if (chance <= Upgrade_Chance)
	//		{
	//			std::vector<Tower> towers = game.getTowers();

	//			std::vector<short> towerIndexes; // This method is for saving the upgrade paths later on
	//			for (short i = 0; i < towers.size(); i++)
	//			{
	//				towerIndexes.push_back(i);
	//			}
	//			std::random_shuffle(towerIndexes.begin(), towerIndexes.end());

	//			short currentTowerIndex = 0;
	//			while (currentTowerIndex < towers.size())
	//			{
	//				Tower tower = towers[towerIndexes[currentTowerIndex]];
	//				if (tower.hasUpgradedTwoPaths())
	//				{
	//					std::array<short, 2> chosenPaths = tower.getChosenPaths();
	//					std::random_shuffle(chosenPaths.begin(), chosenPaths.end());

	//					if (upgradeTower(difficulty, money, &tower, chosenPaths[0])) { break; }
	//					else if (upgradeTower(difficulty, money, &tower, chosenPaths[1])) { break; }
	//					else
	//					{
	//						currentTowerIndex++;
	//						std::random_shuffle(paths.begin(), paths.end());
	//					}
	//				}
	//				else
	//				{
	//					std::random_shuffle(paths.begin(), paths.end());
	//					if (upgradeTower(difficulty, money, &tower, paths[0])) { break; }
	//					else if (upgradeTower(difficulty, money, &tower, paths[1])) { break; }
	//					else if (upgradeTower(difficulty, money, &tower, paths[2])) { break; }
	//					else
	//					{
	//						currentTowerIndex++;
	//						std::random_shuffle(paths.begin(), paths.end());
	//					}
	//				}
	//			}
	//		}
	//		std::cout << "Next Round\n";
	//		INPUT input = Keyboard::keyPress(SPACE_SCAN_CODE);
	//		Clock::wait(.1f);
	//		Keyboard::keyRelease(input);
	//		//game.StartNextRound();
	//		previousRound = nowRound;
	//	}
	//	ReadProcessMemory(handle, healthAddress, &health, sizeof(health), NULL);
	//}
}
