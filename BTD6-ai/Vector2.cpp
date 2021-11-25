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

Vector2 Vector2::operator-(const Vector2 t_right) const
{
	Vector2 newVector2{};
	newVector2.X = X - t_right.X;
	newVector2.Y = Y - t_right.Y;
	return newVector2;
}

Vector2 Vector2::operator+(const Vector2 t_right) const
{
	Vector2 newVector2{};
	newVector2.X = X + t_right.X;
	newVector2.Y = Y + t_right.Y;
	return newVector2;
}

json Vector2::toJson()
{
	return json{ {"X", X}, {"Y", Y} };
}