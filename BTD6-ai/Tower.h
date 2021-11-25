#pragma once
#include <string>
#include <array>

#include <iostream>

#include "Globals.h"
#include "Vector2.h"

class Tower
{
public:
	Tower(const TowerName t_TowerName, const Vector2 t_Position, const double t_RoundBuilt);

	void ChangePosition(const Vector2 t_position);

	bool IsValidPath(const short path);

	Vector2 GetPosition() { return Position; }
	short GetId() { return Id; }

	std::array<short, 3> GetUpgradePath() { return UpgradePath; }

	void IncreasePath(const short path, const double currentRound);
	
	TowerName getTowerName() { return Name; }
private:
	TowerName Name;
	short Id;
	std::array<short, 3> UpgradePath = { 0, 0, 0 };

	short pathsUpgraded = 0;
	short ThirdBuilt = 0;

	double RoundBuilt;
	std::vector<int> PathsUpgraded;
	std::vector<double> RoundsUpgraded;

	Vector2 Position;
};

