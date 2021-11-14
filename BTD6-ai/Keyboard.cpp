#include "Keyboard.h"
#include <Windows.h>
#include <iostream>
#include "Globals.h"

//void Keyboard::Initalise()
//{
//	for (short i = 0; i < 23; i++)
//	{
//		TOWER_SCAN_CODE[i] = MapVirtualKeyA(TOWER_KEY_CODE[i], 4);
//	}
//
//	for (short i = 0; i < 3; i++)
//	{
//		UPGRADE_SCAN_CODE[i] = MapVirtualKeyA(UPGRADE_KEY_CODE[i], 4);
//	}
//
//	SPACE_SCAN_CODE = MapVirtualKeyA(SPACE_KEY_CODE, 4);
//}

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
