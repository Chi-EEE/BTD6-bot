#include "Window.h"

HWND Window::GetHwnd(const char* windowName)
{
	hwnd = FindWindowA(NULL, windowName);
	if (!hwnd)
	{
		std::cout << "Process Window not found: " << windowName << std::endl;
		throw std::runtime_error("Process Window not found!");
	}
	return hwnd;
}

//https://stackoverflow.com/questions/49543452/how-to-get-the-client-window-position-using-the-winapi
RECT Window::GetRect()
{
	MapWindowPoints(hwnd, NULL, (LPPOINT)&windowRect, 2);
	return windowRect;
}

RECT Window::GetSizeRect()
{
	GetClientRect(hwnd, &windowSize); // Right, Bottom 
	return windowSize;
}
