#pragma once
#include <Windows.h>
#include <iostream>

class Window
{
public:
	static HWND GetHwnd(const char* windowName);
    static RECT GetRect(const HWND hwnd);
	static RECT GetSizeRect(const HWND hwnd);

private:
};

