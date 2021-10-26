#include "Keyboard.h"
#include <Windows.h>
#include <iostream>

INPUT Keyboard::keyPress(WORD scanCode)
{
	//Generic key event
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = scanCode;

	input.ki.wVk = NULL;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;

	SendInput(1, &input, sizeof(INPUT));
	return input;
}

void Keyboard::keyRelease(INPUT input)
{
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

	SendInput(1, &input, sizeof(INPUT));
}
