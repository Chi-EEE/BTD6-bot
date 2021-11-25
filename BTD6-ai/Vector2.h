#pragma once
#include "json.hpp"
using json = nlohmann::json;

class Vector2
{
public:
	Vector2();
    Vector2(const int newX, const int newY);

	Vector2 operator-(const Vector2 t_right) const;

	Vector2 operator+(const Vector2 t_right) const;

	json toJson();

	int X;
	int Y;
};

