#include <windows.h>
#include <iostream>
#include <string>
#include <float.h>
#include <array>

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

std::array<int, 7> offsetsFromSimulation = { 0x02006DF8, 0x40, 0xB8, 0x10, 0x298, 0x18, 0x0 };
std::array<int, 4> offsetsToTowerCount = { 0x68, 0x18, 0x30, 0x10 };
std::array<int, 4> offsetsToRound = { 0x270, 0x98, 0x0E0, 0x28 };
std::array<int, 5> offsetsToMoney = { 0x248, 0x18, 0x30, 0x10, 0x28 };
std::array<int, 2> offsetsToHealth = { 0x260, 0x28 };

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
		std::cout << reinterpret_cast<intptr_t>(value) + offsets[i] << "\n";
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
	char* cashValueAddress = memory.ScanForValue(handle, 796.0); // There might be another address with the same value (MONEY ADDRESS IS RNADOM)
	// All other addresses are similar to eachother
	//std::cout << "Cash Address: " << (void*)cashValueAddress << "\n";
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

RECT windowPosition = window.GetRect(); // Window Position
RECT windowSize = window.GetSizeRect(); // Playable Window Size

RECT clientPosition = RECT{ windowPosition.left + x_offset, windowPosition.top + y_offset, windowPosition.right, windowPosition.bottom }; //Top left corner of playable window without hitting ui
RECT clientSize = RECT{ 0, 0, windowSize.right - x_offset - x_minus_offset, windowSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

Vector2 getRandomPosition()
{
	std::cout << "aacvxcv\n";
	int x_axis = clientPosition.left + Random::getValue((LONG)1, clientSize.right);
	int y_axis = clientPosition.top + Random::getValue((LONG)1, clientSize.bottom); // ERROR HERE --------------------------------------------------------------------------------------------
	std::cout << "zxccxzczx\n";
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

int getDifficultyPrice(int difficulty, double defaultPrice)
{
	return static_cast<int>(((defaultPrice * DIFFICULTY[difficulty - 1]) + 2.5) / 5) * 5;
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
		DWORD64 processId = Memory::GetProcessId(L"BloonsTD6.exe");

		//Gets the play screen space size
		Vector2 offPosition = Vector2{ clientSize.right + 200, clientSize.bottom + 101 };

		HANDLE handle = Memory::GetHandle(processId);
		memory.InitaliseMemoryRegions(handle);

		intptr_t moduleAddress = memory.GetModuleBaseAddress(processId, L"GameAssembly.dll");		

		char* simulationAddress = memory.ReadOffsets(handle, (char*)moduleAddress, offsetsFromSimulation);
		std::cout << "Simulation Address: " << (void*)simulationAddress << "\n";

		char* moneyAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToMoney);
		char* healthAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToHealth);
		char* roundAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToRound);
		char* towerCountAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToTowerCount);

		int towerCount = 0;

		double money = 0;
		ReadProcessMemory(handle, moneyAddress, &money, sizeof(money), NULL);

		double health = 0;
		ReadProcessMemory(handle, roundAddress, &health, sizeof(health), NULL);

		double nowRound = 0;
		double previousRound = -1;
		
		short difficulty = 0; // EDIT THISSSSSSSSSSSS

		std::cout << "waiting...\n";
		clock.wait(2.0f);
		std::cout << "reading...\n";
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
					while (towerIndex < ALLOWED_TOWERS.size())
					{
						if (money > getDifficultyPrice(difficulty, TOWER_BASE_COST[static_cast<int>(ALLOWED_TOWERS[towerIndex])]))
						{
							/// <summary>
							/// Gets a random position and tries to place tower in that position. 
							/// </summary>
							while (Tower::getTowerCount() < towerCount && !(currentBuildAttempts >= Build_Attempts))
							{
								game.PlaceTower(getRandomPosition(), ALLOWED_TOWERS[towerIndex], offPosition);
								ReadProcessMemory(handle, towerCountAddress, &towerCount, sizeof(towerCount), NULL);
							}
							break;
						}
						else
							towerIndex++;
					}
				}
				else if (chance <= Upgrade_Chance)
				{
					std::vector<Tower> towers = game.getTowers();

					std::random_shuffle(paths.begin(), paths.end());
					std::random_shuffle(towers.begin(), towers.end());

					short currentTowerIndex = 0;
					while (currentTowerIndex < towers.size())
					{
						Tower tower = towers[currentTowerIndex];
						if (tower.hasUpgradedTwoPaths())
						{
							std::array<short, 2> chosenPaths = tower.getChosenPaths();
							std::random_shuffle(chosenPaths.begin(), chosenPaths.end());

							if (upgradeTower(difficulty, money, &towers[currentTowerIndex], chosenPaths[0])) { break; }
							else if (upgradeTower(difficulty, money, &towers[currentTowerIndex], chosenPaths[1])) { break; }
							else
							{
								currentTowerIndex++;
								std::random_shuffle(paths.begin(), paths.end());
							}
						}
						else
						{
							if (upgradeTower(difficulty, money, &towers[currentTowerIndex], paths[0])) { break; }
							else if (upgradeTower(difficulty, money, &towers[currentTowerIndex], paths[1])) { break; }
							else if (upgradeTower(difficulty, money, &towers[currentTowerIndex], paths[2])) { break; }
							else
							{
								currentTowerIndex++;
								std::random_shuffle(paths.begin(), paths.end());
							}
						}
					}
				}
				game.StartNextRound();
				previousRound = nowRound;
			}
			ReadProcessMemory(handle, healthAddress, &health, sizeof(health), NULL);
		}
		std::cout << "done\n";
	}
	return 1;
}
