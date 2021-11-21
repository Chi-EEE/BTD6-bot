#pragma once
#include "json.hpp"
using json = nlohmann::json;

class Vector2
{
public:
	Vector2();
    Vector2(int newX, int newY);

	json toJson();

	int X;
	int Y;
};

