#include "Window.h"
#include <stdexcept>
#include <limits>
#include <iostream>

HWND Window::GetHwnd(const char* windowName)
{
	HWND hwnd = FindWindowA(NULL, windowName);
	if (!hwnd)
	{
		std::cout << "Process Window not found: " << windowName << std::endl;
	}
	return hwnd;
}

//https://stackoverflow.com/questions/49543452/how-to-get-the-client-window-position-using-the-winapi
RECT Window::GetRect(const HWND hwnd)
{
	RECT windowRect = RECT{};
	MapWindowPoints(hwnd, NULL, (LPPOINT)&windowRect, 2);
	return windowRect;
}

RECT Window::GetSizeRect(const HWND hwnd)
{
	RECT windowSize = RECT{};
	GetClientRect(hwnd, &windowSize); // Right, Bottom 
	return windowSize;
}
