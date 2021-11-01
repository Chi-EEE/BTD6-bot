#include <windows.h>
#include <iostream>
#include <string>
#include <cstdint> // temp

#include "Memory.h"
//#include "ProcessMemory.h"
#include "Window.h"
#include "Random.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Clock.h"

const int x_offset = 21;
const int x_minus_offset = 232;

const int y_offset = 61;

const WORD TOWER_KEY_CODE[23] =
{
    0x55,
    0x51,
    0x57,
    0x45,
    0x52,
    0x54,
    0x59,
    0x5A,
    0x58,
    0x43,
    0x56,
    0x42,
    0x4E,
    0x4D,
    0x41,
    0x53,
    0x44,
    0x46,
    0x47,
    0x48,
    0x4A,
    0x4B,
    0x4C
};

UINT TOWER_SCAN_CODE[23];

const std::string TOWER_NAME[23] =
{
    "Hero",
    "Dart Monkey",
    "Boomerang Monkey",
    "Bomb Shooter",
    "Tack Shooter",
    "Ice Monkey",
    "Glue Gunner",
    "Sniper Monkey",
    "Monkey Sub",
    "Monkey Buccaneer",
    "Monkey Ace",
    "Heli Pilot",
    "Mortar Monkey",
    "Dartling Gunner",
    "Wizard Monkey",
    "Super Monkey",
    "Ninja Monkey",
    "Alchemist",
    "Druid",
    "Banana Farm",
    "Spike Factory",
    "Monkey Village",
    "Engineer Monkey"
};

const int ALLOWED_TOWERS[] =
{
    1,
    3,
    16
};

const int TOWER_UPGRADE[23][3][5] =
{
    //Hero
    {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    }
};

Memory memory = Memory{};
Window window = Window{};
//Random random = Random{};
//Clock clock = Clock{};
//
//Mouse mouse = Mouse{};
//Keyboard keyboard = Keyboard{};

//std::vector<long> offsetsFromCash = { -0x28, -0x10, -0x30, /**/-0x18, -0x248};

int offsetsFromCash1[] = { -0x28, -0x10 };
int offsetsFromCash2[] = { -0x30, -0x18, -0x248 };
//std::vector<T>
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

char* getSimulationAddress(HANDLE handle)
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

int main()
{
	for (short i = 0; i < 23; i++)
	{
		TOWER_SCAN_CODE[i] = MapVirtualKeyA(TOWER_KEY_CODE[i], 4);
	}

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

		//Gets the play screen spze size
		const int x_out = clientSize.right + 200;
		const int y_out = clientSize.bottom + 101;

		HANDLE handle = Memory::GetHandle(processId);
		memory.InitaliseMemoryRegions(handle);
		//memory.GetModuleBaseAddress(processId, L"BloonsTD6.exe");
		int previousRound = 0;

		//reinterpret_cast<std::uintptr_t> method to convert char* to number / void* to string
		// (void*) converts char* to address string

		std::cout << (void*)getSimulationAddress(handle) << "\n";

		//cashValueAddress = memory.ScanForValue(handle, 650.0);// reinterpret_cast<uintptr_t>(address) + offsetsFromCash[2]
		/*if (cashValueAddress)
		{
			std::cout << (void*)cashValueAddress << "\n";
			char* cashManagerAddress = nullptr;
			cashManagerAddress = memory.ScanForValue(handle, reinterpret_cast<uintptr_t>(cashValueAddress) + offsetsFromCash[0]);
			if (cashManagerAddress)
			{
				char* entryAddress = nullptr;
				entryAddress = memory.ScanForValue(handle, reinterpret_cast<uintptr_t>(cashManagerAddress) + offsetsFromCash[1]);
				if (entryAddress)
				{
					std::cout << (void*)entryAddress << "\n";
					char* dictonaryAddresses = nullptr;
					dictonaryAddresses = memory.ScanForValue(handle, reinterpret_cast<uintptr_t>(entryAddress) + offsetsFromCash[2]);
					if (dictonaryAddresses)
					{
						std::cout << (void*)dictonaryAddresses << "\n";
					}
				}
			}
		}*/
	}
	return 1;
}
