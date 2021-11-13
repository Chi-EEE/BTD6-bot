#include <windows.h>
#include <iostream>
#include <string>
#include <float.h>
#include <array>
#include <string.h>

#include "Globals.h"

#include "Game.h"
#include "Bot.h"

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
	Bot bot = Bot{ &game };
	bot.run(&game);
	return 1;
}
