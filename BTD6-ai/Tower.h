#pragma once
#include <string>
#include <array>

#include <iostream>

#include "Globals.h"
#include "Vector2.h"

class Tower
{
public:
	Tower(TowerName t_towerName, Vector2 t_position);

	void ChangePosition(Vector2 t_position);

	bool IsValidPath(short path);

	Vector2 GetPosition() { return Position; }
	int GetId() { return Id; }

	std::array<short, 3> GetUpgradePath() { return UpgradePath; }
	void IncreasePath(short path) { UpgradePath[path]++; }

	TowerName getTowerName() { return Name; }

private:
	TowerName Name;
	int Id;
	std::array<short, 3> UpgradePath = { };

	Vector2 Position;

};

