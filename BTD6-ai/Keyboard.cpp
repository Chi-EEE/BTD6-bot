#include "Keyboard.h"
#include <Windows.h>
#include <iostream>

void Keyboard::keyPress(WORD keyCode)
{
	//Generic key event
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	input.ki.wVk = keyCode;
	input.ki.dwFlags = 0;

	SendInput(1, &input, sizeof(INPUT));
	std::cout << "pressed\n";
}

void Keyboard::keyRelease(WORD keyCode)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	input.ki.wVk = keyCode;
	input.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &input, sizeof(INPUT));
}
