#include <windows.h>
#include <iostream>
#include <string>

#include "ProcessMemory.h"
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

//GameAssembly.dll+23C7EE0 0 98 e0
//// 28/30
//DWORD64 money_firstOffset = 0x021071E8;
//DWORD64 money_finalOffset = 0x28;
//std::vector<DWORD64> money_offsets = { 0x80, 0x218, 0x230, 0x18, 0x30, 0x10 };

DWORD64 money_firstOffset = 0x021071E8;
DWORD64 money_finalOffset = 0x28;
std::vector<DWORD64> money_offsets = { 0x48, 0xA8, 0x230, 0x18, 0x30, 0x10 };


const DWORD64 lives_firstOffset = 0x023E3F70;
const DWORD64 lives_finalOffset = 0x20;
const std::vector<DWORD64> lives_offsets = { 0xB8, 0x0, 0x198, 0x68, 0x30, 0x20 };

const DWORD64 rounds_firstOffset = 0x23C7EE0;
const DWORD64 rounds_finalOffset = 0x30;
const std::vector<DWORD64> rounds_offsets = { 0x0, 0x98, 0xE0 };
//const DWORD64 rounds_firstOffset = 0x023E3F70;
//const DWORD64 rounds_finalOffset = 0x20;
//const std::vector<DWORD64> rounds_offsets = { 0xB8, 0x0, 0x198, 0x68, 0x28, 0x20 };

int main()
{
    Window window = Window{};
    ProcessMemory memory = ProcessMemory{};
    Random random = Random{};
    Clock clock = Clock{};

    Mouse mouse = Mouse{};
    Keyboard keyboard = Keyboard{};

	for (short i = 0; i < 23; i++)
	{
		TOWER_SCAN_CODE[i] = MapVirtualKeyA(TOWER_KEY_CODE[i], 4);
	}

    //std::cout << "Waiting 5 seconds" << std::endl;
    //clock.wait(5.f);
    HWND hwnd = window.GetHwnd("BloonsTD6");
	if (hwnd)
	{
		DWORD64 processId = memory.GetProcessId(L"BloonsTD6.exe");
		//uintptr_t moduleBaseAddress = memory.GetModuleBaseAddress(processId, L"GameAssembly.dll");

		RECT clientPosition = window.GetRect();
		RECT clientSize = window.GetSizeRect();

		clientPosition = RECT{ clientPosition.left + x_offset, clientPosition.top + y_offset, clientPosition.right, clientPosition.bottom }; //Top left corner of window
		clientSize = RECT{ 0, 0, clientSize.right - x_offset - x_minus_offset, clientSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

		const int x_out = clientSize.right + 200;
		const int y_out = clientSize.bottom + 101;
		/*  std::cout << clientPosition.left << " " << clientPosition.top << std::endl;
		  std::cout << clientSize.right << " " << clientSize.bottom << std::endl;*/
		HANDLE handle = memory.GetHandle(processId);
		memory.InitaliseMemoryRegions(handle);
		//memory.GetModuleBaseAddress(processId, L"BloonsTD6.exe");
		int previousRound = 0;

		// (void*) converts char* to address string
		while (true)
		{
			for (Region region : memory.GetMemoryRegions())
			{
				char* address = memory.ScanForDoubleValue(handle, 650, region.BaseAddress, region.Size);
				if (address)
				{
					std::cout << (void*)address << "\n";
					/*
					double money;
					while (true)
					{
						ReadProcessMemory(handle, address, &money, sizeof(money), NULL);
						while (money > 500)
						{
							std::cout << money << "\n";
							const int TOWER_INDEX = ALLOWED_TOWERS[random.GetValue(1, 3) - 1];
							int x_axis = clientPosition.left + random.GetValue(1, clientSize.right);
							int y_axis = clientPosition.top + random.GetValue(1, clientSize.bottom);
							std::cout << "Placing " << TOWER_NAME[TOWER_INDEX] << " at " << x_axis << ", " << y_axis << std::endl;

							INPUT input = keyboard.keyPress(TOWER_SCAN_CODE[TOWER_INDEX]);

							mouse.setPosition(x_axis, y_axis);
							mouse.leftMouseDown();

							clock.wait(.1f);
							keyboard.keyRelease(input);
							mouse.leftMouseUp();
							mouse.setPosition(x_out, y_out);

							ReadProcessMemory(handle, address, &money, sizeof(money), NULL);

							clock.wait(0.5f);
						}
					}
					// */
				}
			}
		}
	}
	return 1;
}
