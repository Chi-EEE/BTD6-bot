#include <windows.h>
#include <iostream>

#include "Mouse.h"
#include "Keyboard.h"
#include "Clock.h"
int main()
{
    //Mouse m = Mouse{};
    Keyboard k = Keyboard{};
    Clock c = Clock{};

    c.wait(4.0f);
    std::cout << "Pressed Q!\n";
    k.keyPress(0x51);
    c.wait(1.5f);
    std::cout << "Released Q!\n";
    k.keyRelease(0x51);
}