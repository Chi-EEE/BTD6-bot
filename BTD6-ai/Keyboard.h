#pragma once
#include <Windows.h>

class Keyboard
{
public:
	INPUT keyPress(WORD scanCode);
	void keyRelease(INPUT input);
private:
};