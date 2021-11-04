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

	void UpgradePath(short path);
	short getLatestUpgradePath(short path) { return upgradePath[path - 1]; }

	TowerName getTowerName() { return towerName; }

	short getChosenPath(short index) { return pathChosen[index]; }
	std::array<short, 2> getChosenPaths() { return pathChosen; }
	bool hasUpgradedTwoPaths() { return upgradedTwoPaths; }

	static short getTowerCount() { return TowerCount; }
private:
	static short TowerCount;

	TowerName towerName;

	short upgradePath[3] = { 0, 0, 0 };

	std::array<short, 2> pathChosen = { 0, 0 };
	bool upgradedTwoPaths = false;

	Vector2 position;
};

