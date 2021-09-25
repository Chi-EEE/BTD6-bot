#pragma once
#include <Windows.h>
#include <iostream>

class Window
{
public:
	HWND GetHwnd(const char* windowName);
	RECT GetRect();
	RECT GetSizeRect();

private:
	HWND hwnd;
	RECT windowRect;
	RECT windowSize;
};

