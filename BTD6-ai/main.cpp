#include <windows.h>
#include <iostream>
#include <string>

#include "Globals.h"

#include "Game.h"
#include "Memory.h"
#include "Window.h"
#include "Random.h"
#include "Mouse.h"
#include "Clock.h"
#include "Keyboard.h"

const int x_offset = 21;
const int x_minus_offset = 232;

const int y_offset = 61;

const TowerName ALLOWED_TOWERS[] =
{
	TowerName::Ninja_Monkey,
	TowerName::Bomb_Shooter
};

std::vector<int> offsetsFromSimulation = { 0x021363A0, 0x18, 0xB8, 0x10, 0x3C8, 0x18, 0x0 };
std::vector<int> offsetsToMoney = { 0x248, 0x18, 0x30, 0x10, 0x28 };
std::vector<int> offsetsToHealth = { 0x260, 0x28 };

Memory memory = Memory{};
Window window = Window{};
Game game = Game{};

// Following functions aren't needed in program regularly

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
	char* cashValueAddress = memory.ScanForValue(handle, 650.0);
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
		//uintptr_t moduleBaseAddress = memory.GetModuleBaseAddress(processId, L"GameAssembly.dll");

		RECT clientPosition = window.GetRect();
		RECT clientSize = window.GetSizeRect();

		clientPosition = RECT{ clientPosition.left + x_offset, clientPosition.top + y_offset, clientPosition.right, clientPosition.bottom }; //Top left corner of window
		clientSize = RECT{ 0, 0, clientSize.right - x_offset - x_minus_offset, clientSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

		//Gets the play screen space size
		const int x_out = clientSize.right + 200;
		const int y_out = clientSize.bottom + 101;

		HANDLE handle = Memory::GetHandle(processId);
		memory.InitaliseMemoryRegions(handle);

		intptr_t moduleAddress = memory.GetModuleBaseAddress(processId, L"GameAssembly.dll");
		int previousRound = 0;
		
		std::cout << (void*)getSimulationAddress(handle) << "\n";

		char* simulationAddress = memory.ReadOffsets(handle, (char*)moduleAddress, offsetsFromSimulation);
		int difficulty = 0;
		std::cout << (void*)simulationAddress << "\n";
		// DO IN LOOP
		bool roundEnded = false;
		while (true)
		{
			roundEnded = false;
			if (roundEnded)
			{
				int chance = Random::getValue(1, 100);
				if (chance <= Buy_Chance)
				{

				}
				else if (chance <= Upgrade_Chance)
				{
					int randomTower = {};
					std::vector<short> paths = { 1, 2, 3 };
					std::random_shuffle(paths.begin(), paths.end());

					bool upgradedTower = game.UpgradeTower(1, paths[0]);
					while (!upgradedTower)
					{
						paths.
						bool upgradedTower = game.UpgradeTower(1, Random::getValue(1, 3));
					}
				}
				game.StartNextRound();
			}
		}
		/*char* moneyAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToMoney);
		char* healthAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToHealth);
		std::cout << "2";
		clock.wait(2.0f);
		double money;
		double health;
		while (true)
		{
			ReadProcessMemory(handle, healthAddress, &health, sizeof(health), NULL);
			if (health <= 0)
			{
				std::cout << "Stopped!\n";
				break;
			}
			ReadProcessMemory(handle, moneyAddress, &money, sizeof(money), NULL);
			if (money > 500)
			{
				int x_axis = clientPosition.left + random.GetValue(1, clientSize.right);
				int y_axis = clientPosition.top + random.GetValue(1, clientSize.bottom);
			}
		}*/
		// 
	}
	return 1;
}
