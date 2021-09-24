#include <windows.h>
#include <iostream>

#include "Memory.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Clock.h"
int main()
{
    //Mouse m = Mouse{};
    //Keyboard k = Keyboard{};
    //Clock c = Clock{};
    Memory m = Memory{};
    DWORD64 processId = m.GetProcessId("BloonsTD6");

    uintptr_t moduleBaseAddress = m.GetModuleBaseAddress(processId, L"GameAssembly.dll");

    HANDLE handle = m.GetHandle(processId);

    DWORD64 firstOffset = 0x020F2890;
    DWORD64 finalOffset = 0x28;
    std::vector<DWORD64> offsets = { 0x30, 0x88, 0x18, 0xD8, 0x30, 0x10 };

    double money = m.GetDoubleFromOffsets(handle, moduleBaseAddress, firstOffset, offsets, finalOffset);
    std::cout << money << std::endl;
    //c.wait(4.0f);
    //std::cout << "Pressed Q!\n";
    //k.keyPress(0x51);
    //c.wait(1.5f);
    //std::cout << "Released Q!\n";
    //k.keyRelease(0x51);
    return 1;
}
