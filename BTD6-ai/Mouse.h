#pragma once
#include <Windows.h>

#include "Vector2.h"
class Mouse
{
public:
	static void setPosition(int t_x, int t_y);
	static void setPosition(Vector2 t_position);
	static void leftMouseDown();
	static void leftMouseUp();
private:

};

