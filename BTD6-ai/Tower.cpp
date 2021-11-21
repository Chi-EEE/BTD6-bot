#include "Tower.h"

#include "Globals.h"
#include "Clock.h"
#include "Mouse.h"
#include "Keyboard.h"
Tower::Tower(TowerName t_TowerName, Vector2 t_Position, double t_RoundBuilt)
{ 
	Name = t_TowerName;
	Id = static_cast<int>(t_TowerName);
	Position = t_Position;
	RoundBuilt = t_RoundBuilt;
}

void Tower::ChangePosition(Vector2 t_position)
{
	Position = t_position;
}

bool Tower::IsValidPath(short path)
{
	if (pathsUpgraded >= 2)
	{
		if (UpgradePath[path] && UpgradePath[path] < 5)
		{
			// Return if third upgraded isn't here or third upgrade is equal to path
			return !ThirdBuilt || ThirdBuilt == path + 1;
		}
	}
	else if (!UpgradePath[path])
	{
		return true;
	}
	return false;
}

/// <summary>
/// Increases the upgrade path
/// [MAKE SURE TO CALL IsValidPath before this function]
/// </summary>
/// <param name="path"></param>
void Tower::IncreasePath(short path, double currentRound)
{
	pathsUpgraded++;
	UpgradePath[path]++;
	if (UpgradePath[path] == 3)
	{
		ThirdBuilt = path + 1;
	}
	PathsUpgraded.push_back(path);
	RoundsUpgraded.push_back(currentRound);
}
