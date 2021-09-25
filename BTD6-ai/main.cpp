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

DWORD64 money_firstOffset = 0x020F2890;
DWORD64 money_finalOffset = 0x28;
std::vector<DWORD64> money_offsets = { 0x30, 0x88, 0x18, 0xD8, 0x30, 0x10 };

const DWORD64 lives_firstOffset = 0x023E3F70;
const DWORD64 lives_finalOffset = 0x20;
const std::vector<DWORD64> lives_offsets = { 0xB8, 0x0, 0x198, 0x68, 0x30, 0x20 };

const DWORD64 rounds_firstOffset = 0x023E3F70;
const DWORD64 rounds_finalOffset = 0x20;
const std::vector<DWORD64> rounds_offsets = { 0xB8, 0x0, 0x198, 0x68, 0x28, 0x20 };

int main()
{
    Window window = Window{};
    ProcessMemory memory = ProcessMemory{};
    Random random = Random{};
    Clock clock = Clock{};

    Mouse mouse = Mouse{};
    Keyboard keyboard = Keyboard{};

    std::cout << "Waiting 5 seconds" << std::endl;
    clock.wait(5.f);
    HWND hwnd = window.GetHwnd("BloonsTD6");
    if (hwnd)
    {
        DWORD64 processId = memory.GetProcessId(hwnd);
        uintptr_t moduleBaseAddress = memory.GetModuleBaseAddress(processId, L"GameAssembly.dll");
        HANDLE handle = memory.GetHandle(processId);

        RECT clientPosition = window.GetRect();
        RECT clientSize = window.GetSizeRect();

        clientPosition = RECT{ clientPosition.left + x_offset, clientPosition.top + y_offset, clientPosition.right, clientPosition.bottom}; //Top left corner of window
        clientSize = RECT{ 0, 0, clientSize.right - x_offset - x_minus_offset, clientSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

        const int x_out = clientSize.left + 178;
        const int y_out = clientSize.top + 101;
        std::cout << clientPosition.left << " " << clientPosition.top << std::endl;
        std::cout << clientSize.right << " " << clientSize.bottom << std::endl;

        int times = 10;
        int previousRound = 0;
        double lives = memory.GetFloatFromOffsets(handle, moduleBaseAddress, lives_firstOffset, lives_offsets, lives_finalOffset);
        while (times >= 1)
        {
            int currentRound = memory.GetIntFromOffsets(handle, moduleBaseAddress, rounds_firstOffset, rounds_offsets, rounds_finalOffset);
            if (previousRound != currentRound)
            {
                double money = memory.GetDoubleFromOffsets(handle, moduleBaseAddress, money_firstOffset, money_offsets, money_finalOffset);
                while (money > 500)
                {
                    const int TOWER_INDEX = ALLOWED_TOWERS[random.GetValue(1, 2) - 1];
                    int x_axis = clientPosition.left + random.GetValue(1, clientSize.right);
                    int y_axis = clientPosition.top + random.GetValue(1, clientSize.bottom);
                    std::cout << "Placing " << TOWER_NAME[TOWER_INDEX] << " at " << x_axis << ", " << y_axis << std::endl;

                    keyboard.keyPress(TOWER_KEY_CODE[TOWER_INDEX]);

                    mouse.setPosition(x_axis, y_axis);
                    mouse.leftMouseDown();

                    clock.wait(0.1f);
                    keyboard.keyRelease(TOWER_KEY_CODE[TOWER_INDEX]);
                    mouse.leftMouseUp();
                    mouse.setPosition(x_out, y_out);

                    clock.wait(0.5f);
                    times--;
                }
            }
            clock.wait(0.5f);
            lives = memory.GetFloatFromOffsets(handle, moduleBaseAddress, lives_firstOffset, lives_offsets, lives_finalOffset);
            if (lives <= 0)
            {
                break;
            }
        }
    }
    return 1;
}
