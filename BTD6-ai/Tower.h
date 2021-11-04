#pragma once
#include <string>

#include "Globals.h"
#include "Vector2.h"

class Tower
{
public:
	Tower(TowerName t_towerName, Vector2 t_position);

	void ChangePosition(Vector2 t_position);

	bool UpgradePath(short path);
	short* getUpgradePaths() { return upgradePath; }
	short getLatestUpgradePath(short path) { return upgradePath[path - 1]; }

	TowerName getTowerName() { return towerName; }

	short getChosenPath(short index) { return pathChosen[index]; }
	
	static int getTowerCount() { return TowerCount; }
private:
	static short TowerCount;

	TowerName towerName;

	short upgradePath[3] = { 0, 0, 0 };

	short pathChosen[2] = { 0, 0 };

	Vector2 position;
};

