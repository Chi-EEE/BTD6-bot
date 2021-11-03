#pragma once
#include <string>

#include "Vector2.h"

class Tower
{
public:
	Tower(int t_towerIndex, std::string t_towerName, Vector2 t_position);

	void ChangePosition(Vector2 t_position);

	bool UpgradePath(short path);
	short* getUpgradePaths() { return upgradePath; }
	short getLatestUpgradePath(short path) { return upgradePath[path - 1]; }

	short getTowerIndex() { return towerIndex; }

	static int getTowerCount() { return TowerCount; }
private:
	static short TowerCount;

	int towerIndex;
	std::string name;

	short upgradePath[3] = { 0, 0, 0 };

	short pathChosen[2] = { 0, 0 };

	Vector2 position;
};

