#include "Mouse.h"

void Mouse::setPosition(const int t_x, const int t_y)
{
	SetCursorPos(t_x, t_y);
}

void Mouse::leftMouseDown()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void Mouse::leftMouseUp()
{
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
