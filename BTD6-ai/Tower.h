#pragma once
#include <string>
#include <array>

#include "Globals.h"
#include "Vector2.h"

class Tower
{
public:
	Tower(TowerName t_towerName, Vector2 t_position);

	void ChangePosition(Vector2 t_position);

	bool IsValidPath(short path);

	bool UpgradeTower(double money, short path);

	TowerName getTowerName() { return Name; }

private:
	TowerName Name;
	int Id;

	Vector2 Position;

	std::array<short, 3> UpgradePath = { 0, 0, 0 };
};

