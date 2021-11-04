#include "Tower.h"
short Tower::TowerCount = 0;

Tower::Tower(TowerName t_towerName, Vector2 t_position)
{
	TowerCount++;
	towerName = t_towerName;
	position = t_position;
}

void Tower::ChangePosition(Vector2 t_position)
{
	position = t_position;
}

void Tower::UpgradePath(short path)
{
	bool equalPath = (pathChosen[0] == path || pathChosen[1] == path);
	if (!equalPath && !upgradedTwoPaths)
	{
		if (!pathChosen[0]) { pathChosen[0] = path; upgradePath[path - 1]++; }
		else if (!pathChosen[1]) { pathChosen[1] = path; upgradePath[path - 1]++; upgradedTwoPaths = true; }
	}
	else
		upgradePath[path - 1]++;
}