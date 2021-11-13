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

//int getDifficultyPrice(int difficulty, double defaultPrice)
//{
//	return static_cast<int>(((defaultPrice * DIFFICULTY[difficulty]) + 2.5) / 5) * 5;
//}

//Vector2 getRandomPosition()
//{
//	int x_axis = clientPosition.left + Random::getValue((int)1, (int)(clientSize.right));
//	int y_axis = clientPosition.top + Random::getValue((int)1, (int)(clientSize.bottom));
//	return Vector2{ x_axis, y_axis };
//}

//bool upgradeTower(int difficulty, int money, Tower *currentTower, int path)
//{ // RANDOMLY CHOOSE PATH IN ARRAY
//	
//	if (path != currentTower->getChosenPath(1) && path != currentTower->getChosenPath(2))
//	{
//		return false;
//	}
//	int nextUpgradePath = currentTower->getLatestUpgradePath(path) + 1;
//	if (nextUpgradePath > 5)
//	{
//		return false;
//	}
//	if (money >= getDifficultyPrice(difficulty, TOWER_UPGRADE[static_cast<int>(currentTower->getTowerName())][path][nextUpgradePath]))
//	{
//		game.UpgradeTower(*currentTower, path);
//		return true;
//	}
//	return false;
//}

int main()
{
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
	//Clock::wait(5.f);
	//HWND hwnd = window.GetHwnd("BloonsTD6");
	//if (hwnd)
	//{
	//	//windowPosition = window.GetRect(); // Window Position
	//	//windowSize = window.GetSizeRect(); // Playable Window Size

	//	//clientPosition = RECT{ windowPosition.left + x_offset, windowPosition.top + y_offset, windowPosition.right, windowPosition.bottom }; //Top left corner of playable window without hitting ui
	//	//clientSize = RECT{ 0, 0, windowSize.right - x_offset - x_minus_offset, windowSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

	//	DWORD64 prcocessId = Memory::GetProcessId(L"BloonsTD6.exe");

	//	//Gets the play screen space size
	//	//Vector2 offPosition = Vector2{ clientSize.right + 200, clientSize.bottom + 101 };

	//	//HANDLE handle = Memory::GetHandle(processId);
	//	//memory.InitaliseMemoryRegions(handle);
	//	//std::cout << (void*)getSimulationAddress(handle) << "\n";
	//	
	//}
	return 1;
}
