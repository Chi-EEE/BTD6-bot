#include <windows.h>
#include <iostream>

#include "Memory.h"
#include "Window.h"
#include "Random.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Clock.h"
int main()
{
    //Mouse m = Mouse{};
    //Keyboard k = Keyboard{};
    //Clock c = Clock{};
    Memory m = Memory{};
    Window window = Window{};
    Random r = Random{};

    HWND hwnd = window.GetHwnd("BloonsTD6");
    RECT windowRect = window.GetRect();
    RECT windowSize = window.GetSizeRect();
    
    std::cout << windowSize.right << " " << windowSize.bottom << std::endl;
    std::cout << r.GetValue(1, 100);
    //DWORD64 processId = m.GetProcessId(hwnd);

    //uintptr_t moduleBaseAddress = m.GetModuleBaseAddress(processId, L"GameAssembly.dll");

    //HANDLE handle = m.GetHandle(processId);

    //DWORD64 money_firstOffset = 0x020F2890;
    //DWORD64 money_finalOffset = 0x28;
    //std::vector<DWORD64> money_offsets = { 0x30, 0x88, 0x18, 0xD8, 0x30, 0x10 };

    //double money = m.GetDoubleFromOffsets(handle, moduleBaseAddress, money_firstOffset, money_offsets, money_finalOffset);
    //std::cout << money << std::endl;

    //DWORD64 lives_firstOffset = 0x023E3F70;
    //DWORD64 lives_finalOffset = 0x20;
    //std::vector<DWORD64> lives_offsets = { 0xB8, 0x0, 0x198, 0x68, 0x30, 0x20};

    //double lives = m.GetFloatFromOffsets(handle, moduleBaseAddress, lives_firstOffset, lives_offsets, lives_finalOffset);
    //std::cout << lives << std::endl;
    //c.wait(4.0f);
    //std::cout << "Pressed Q!\n";
    //k.keyPress(0x51);
    //c.wait(1.5f);
    //std::cout << "Released Q!\n";
    //k.keyRelease(0x51);
    return 1;
}
