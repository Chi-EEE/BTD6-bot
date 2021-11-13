#pragma once
#include <Windows.h>

class Keyboard
{
public:
	static INPUT keyPress(WORD scanCode);
	static void keyRelease(INPUT input);
private:
};