#pragma once
#include <string>
#include <array>

#include <iostream>

#include "Globals.h"
#include "Vector2.h"

class Tower
{
public:
	Tower(TowerName t_TowerName, Vector2 t_Position, double t_RoundBuilt);

	void ChangePosition(Vector2 t_position);

	bool IsValidPath(short path);

	Vector2 GetPosition() { return Position; }
	int GetId() { return Id; }

	std::array<short, 3> GetUpgradePath() { return UpgradePath; }

	void IncreasePath(short path, double currentRound);
	
	TowerName getTowerName() { return Name; }
private:
	TowerName Name;
	int Id;
	std::array<short, 3> UpgradePath = { 0, 0, 0 };

	short pathsUpgraded = 0;
	short ThirdBuilt = 0;

	double RoundBuilt;
	std::vector<int> PathsUpgraded;
	std::vector<double> RoundsUpgraded;

	Vector2 Position;
};

