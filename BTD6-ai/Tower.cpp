#include "Tower.h"
short Tower::TowerCount = 0;

Tower::Tower(int t_towerIndex, std::string t_towerName, Vector2 t_position)
{
	TowerCount++;
	towerIndex = t_towerIndex;
	name = t_towerName;
	position = t_position;
}

void Tower::ChangePosition(Vector2 t_position)
{
	position = t_position;
}

bool Tower::UpgradePath(short path)
{
	if (!pathChosen[0]) { pathChosen[0] = path; }
	else if (!pathChosen[1]) { pathChosen[1] = path; }

	if (pathChosen[0] == path || pathChosen[1] == path)
	{
		if (upgradePath[path - 1] < 6)
		{
			upgradePath[path - 1]++;
			return true;
		}
	}
	return false;
}