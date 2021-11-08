#include <windows.h>
#include <iostream>
#include <string>
#include <float.h>
#include <array>
#include <string.h>

#include "Globals.h"

#include "Game.h"
#include "Tower.h"

#include "Memory.h"
#include "Window.h"
#include "Random.h"
#include "Mouse.h"
#include "Clock.h"
#include "Keyboard.h"

const int x_offset = 21;
const int x_minus_offset = 232;

const int y_offset = 61;

std::vector<TowerName> ALLOWED_TOWERS =
{
	TowerName::Ninja_Monkey,
	TowerName::Bomb_Shooter
};
std::array<short, 3> paths = { 1, 2, 3 };

Memory memory = Memory{};
Window window = Window{};
Game game = Game{};

// Following functions aren't needed in program regularly
/*
//reinterpret_cast<std::uintptr_t> method to convert char* to number / void* to string
int offsetsFromCash1[] = { -0x28, -0x10 };
int offsetsFromCash2[] = { -0x30, -0x18, -0x248 };
template <typename T, size_t size> char* scanLinear(HANDLE handle, char* startingValue, const T(&offsets)[size])
{
	char* value = startingValue;
	for (size_t i = 0; i < size; i++)
	{
		value = memory.ScanForValue(handle, reinterpret_cast<intptr_t>(value) + offsets[i]);
	}
	return value;
}

/// <summary>
/// This method should be used whenever there's an update to save time searching for pointer to simulation
/// </summary>
/// <param name="handle"></param>
/// <returns></returns>
char* getSimulationAddress(HANDLE handle) // Instead of looking for sim, get static pointer to it? It might work
{
	unsigned long long cashValueINTString;

	std::cout << "Enter Money Address: ";
	std::cin >> cashValueINTString;

	char* cashValueAddress = (char*)(cashValueINTString);

	char* entryOffsetAddress = scanLinear(handle, cashValueAddress, offsetsFromCash1);

	std::vector<char*> dictonaryAddresses = {};
	memory.ScanForValues(handle, (reinterpret_cast<intptr_t>(entryOffsetAddress) + offsetsFromCash2[0]), dictonaryAddresses);

	char* validDictonaryAddress = nullptr;
	for (char* dictonaryAddress : dictonaryAddresses)
	{
		char* simulationOffsetAddress = memory.ScanForValue(handle, (reinterpret_cast<intptr_t>(dictonaryAddress) + offsetsFromCash2[1]));
		if (simulationOffsetAddress)
		{
			validDictonaryAddress = simulationOffsetAddress;
			break;
		}
	}
	return (char*)(reinterpret_cast<intptr_t>(validDictonaryAddress + offsetsFromCash2[2]));
}
// */

RECT windowPosition;
RECT windowSize;

RECT clientPosition;
RECT clientSize;

int getDifficultyPrice(int difficulty, double defaultPrice)
{
	return static_cast<int>(((defaultPrice * DIFFICULTY[difficulty]) + 2.5) / 5) * 5;
}

Vector2 getRandomPosition()
{
	int x_axis = clientPosition.left + Random::getValue((int)1, (int)(clientSize.right));
	int y_axis = clientPosition.top + Random::getValue((int)1, (int)(clientSize.bottom));
	return Vector2{ x_axis, y_axis };
}

bool upgradeTower(int difficulty, int money, Tower *currentTower, int path)
{ // RANDOMLY CHOOSE PATH IN ARRAY
	
	if (path != currentTower->getChosenPath(1) && path != currentTower->getChosenPath(2))
	{
		return false;
	}
	int nextUpgradePath = currentTower->getLatestUpgradePath(path) + 1;
	if (nextUpgradePath > 5)
	{
		return false;
	}
	if (money >= getDifficultyPrice(difficulty, TOWER_UPGRADE[static_cast<int>(currentTower->getTowerName())][path][nextUpgradePath]))
	{
		game.UpgradeTower(*currentTower, path);
		return true;
	}
	return false;
}

int main()
{
	Clock clock = Clock{}; // Should be outside function
	for (short i = 0; i < 23; i++)
	{
		TOWER_SCAN_CODE[i] = MapVirtualKeyA(TOWER_KEY_CODE[i], 4);
	}

	for (short i = 0; i < 3; i++)
	{
		UPGRADE_SCAN_CODE[i] = MapVirtualKeyA(UPGRADE_KEY_CODE[i], 4);
	}

	SPACE_SCAN_CODE = MapVirtualKeyA(SPACE_KEY_CODE, 4);
	//std::cout << "Waiting 5 seconds" << std::endl;
	//clock.wait(5.f);
	HWND hwnd = window.GetHwnd("BloonsTD6");
	if (hwnd)
	{
		Keyboard keyboard;
		Mouse mouse;

		windowPosition = window.GetRect(); // Window Position
		windowSize = window.GetSizeRect(); // Playable Window Size

		clientPosition = RECT{ windowPosition.left + x_offset, windowPosition.top + y_offset, windowPosition.right, windowPosition.bottom }; //Top left corner of playable window without hitting ui
		clientSize = RECT{ 0, 0, windowSize.right - x_offset - x_minus_offset, windowSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

		DWORD64 processId = Memory::GetProcessId(L"BloonsTD6.exe");

		//Gets the play screen space size
		Vector2 offPosition = Vector2{ clientSize.right + 200, clientSize.bottom + 101 };

		HANDLE handle = Memory::GetHandle(processId);
		memory.InitaliseMemoryRegions(handle);
		std::cout << (void*)getSimulationAddress(handle) << "\n";
		/*
		intptr_t moduleAddress = memory.GetModuleBaseAddress(processId, L"GameAssembly.dll");		

		char* simulationAddress = memory.ReadOffsets(handle, (char*)moduleAddress, offsetsFromSimulation);
		std::cout << "Simulation Address: " << (void*)simulationAddress << "\n";

		char* moneyAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToMoney);
		char* healthAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToHealth);
		char* roundAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToRound);
		char* towerCountAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToTowerCount);
		std::cout << (void*)towerCountAddress << "\n";
		int towerCount = 0;

		double money = 0;
		ReadProcessMemory(handle, moneyAddress, &money, sizeof(money), NULL);

		double health = 0;
		ReadProcessMemory(handle, healthAddress, &health, sizeof(health), NULL);

		double nowRound = 0;
		double previousRound = -1;
		
		short difficulty = 0; // EDIT THISSSSSSSSSSSS

		std::cout << "waiting...\n";
		clock.wait(6.0f);
		std::cout << "reading...\n";
		std::cout << "Health: " << health << "\n";
		std::cout << "Round: " << nowRound << "\n";
		while (health > 0 && nowRound < DIFFICULTY_ROUND[difficulty])
		{
			ReadProcessMemory(handle, roundAddress, &nowRound, sizeof(nowRound), NULL);
			if (nowRound > previousRound) // Runs the following when the round finishes
			{
				ReadProcessMemory(handle, moneyAddress, &money, sizeof(money), NULL);

				int chance = Random::getValue(1, 100);
				// Don't need to go into big loop because the round didn't start
				if (chance <= Buy_Chance)
				{
					std::random_shuffle(ALLOWED_TOWERS.begin(), ALLOWED_TOWERS.end());

					int currentBuildAttempts = 0;
					short towerIndex = 0;
					std::cout << "Placing Tower\n";
					while (towerIndex < ALLOWED_TOWERS.size())
					{
						if (money > getDifficultyPrice(difficulty, TOWER_BASE_COST[static_cast<int>(ALLOWED_TOWERS[towerIndex])]))
						{
							/// <summary>
							/// Gets a random position and tries to place tower in that position. 
							/// </summary>
							TowerName towerName;
							Vector2 position ;
							bool builtTower = false;

							while (towerCount <= Tower::getTowerCount() && !(currentBuildAttempts >= Build_Attempts))
							{
								{ // Place Tower
									position = getRandomPosition();

									towerName = ALLOWED_TOWERS[towerIndex];
									INPUT input = keyboard.keyPress(TOWER_SCAN_CODE[static_cast<int>(towerName)]);

									mouse.setPosition(position);
									mouse.leftMouseDown();

									clock.wait(.1f);
									keyboard.keyRelease(input);
									mouse.leftMouseUp();
									mouse.setPosition(offPosition);
								}

								ReadProcessMemory(handle, towerCountAddress, &towerCount, sizeof(towerCount), NULL);
								currentBuildAttempts++;
								clock.wait(1.f);
								builtTower = true;
							} // Tower Count updates after it first places, example: place then another place and position is at the second place
							if (builtTower && !(currentBuildAttempts >= Build_Attempts))
							{
								std::cout << static_cast<int>(towerName) << "\n";
								Tower newTower = Tower{ towerName, position };
								game.AddTower(newTower);
							}
							std::cout << "Done Building\n";
							break;
						}
						else
							towerIndex++;
					}
				}
				else if (chance <= Upgrade_Chance)
				{
					std::vector<Tower> towers = game.getTowers();

					std::vector<short> towerIndexes; // This method is for saving the upgrade paths later on
					for (short i = 0; i < towers.size(); i++)
					{
						towerIndexes.push_back(i);
					}
					std::random_shuffle(towerIndexes.begin(), towerIndexes.end());

					short currentTowerIndex = 0;
					while (currentTowerIndex < towers.size())
					{
						Tower tower = towers[towerIndexes[currentTowerIndex]];
						if (tower.hasUpgradedTwoPaths())
						{
							std::array<short, 2> chosenPaths = tower.getChosenPaths();
							std::random_shuffle(chosenPaths.begin(), chosenPaths.end());

							if (upgradeTower(difficulty, money, &tower, chosenPaths[0])) { break; }
							else if (upgradeTower(difficulty, money, &tower, chosenPaths[1])) { break; }
							else
							{
								currentTowerIndex++;
								std::random_shuffle(paths.begin(), paths.end());
							}
						}
						else
						{
							std::random_shuffle(paths.begin(), paths.end());
							if (upgradeTower(difficulty, money, &tower, paths[0])) { break; }
							else if (upgradeTower(difficulty, money, &tower, paths[1])) { break; }
							else if (upgradeTower(difficulty, money, &tower, paths[2])) { break; }
							else
							{
								currentTowerIndex++;
								std::random_shuffle(paths.begin(), paths.end());
							}
						}
					}
				}
				std::cout << "Next Round\n";
				INPUT input = keyboard.keyPress(SPACE_SCAN_CODE);
				clock.wait(.1f);
				keyboard.keyRelease(input);
				//game.StartNextRound();
				previousRound = nowRound;
			}
			ReadProcessMemory(handle, healthAddress, &health, sizeof(health), NULL);
		}
		std::cout << "done\n";
		// */
	}
	return 1;
}
