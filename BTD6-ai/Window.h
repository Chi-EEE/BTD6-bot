#pragma once
#include <Windows.h>
#include <iostream>

class Window
{
public:
	static HWND GetHwnd(const char* windowName);
    static RECT GetRect(HWND hwnd);
	static RECT GetSizeRect(HWND hwnd);

private:
};

