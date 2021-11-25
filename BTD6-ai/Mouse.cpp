#include "Mouse.h"

/// <summary>
/// Sets the position of the mouse
/// </summary>
/// <param name="t_x"></param>
/// <param name="t_y"></param>
void Mouse::setPosition(const int t_x, const int t_y)
{
	SetCursorPos(t_x, t_y);
}

void Mouse::setPosition(const Vector2 t_position)
{
	SetCursorPos(t_position.X, t_position.Y);
}

/// <summary>
/// Holds the left mouse button
/// </summary>
void Mouse::leftMouseDown()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

/// <summary>
/// Release the left mouse button
/// </summary>
void Mouse::leftMouseUp()
{
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
