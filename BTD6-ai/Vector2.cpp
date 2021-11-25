#include "Vector2.h"

Vector2::Vector2()
{
	X = 0;
	Y = 0;
}

Vector2::Vector2(const int newX, const int newY)
{
	X = newX;
	Y = newY;
}

json Vector2::toJson()
{
	return json{ {"X", X}, {"Y", Y} };
}