#pragma once
#include <Windows.h>

#include "Vector2.h"
class Mouse
{
public:
	void setPosition(int t_x, int t_y);
	void setPosition(Vector2 t_position);
	void leftMouseDown();
	void leftMouseUp();
private:

};

