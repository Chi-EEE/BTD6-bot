#pragma once
#include <Windows.h>

#include "Vector2.h"
class Mouse
{
public:
	static void setPosition(const int t_x, const int t_y);
	static void setPosition(const Vector2 t_position);
	static void leftMouseDown();
	static void leftMouseUp();
private:

};

