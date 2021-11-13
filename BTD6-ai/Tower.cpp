#include "Tower.h"

#include "Clock.h"
#include "Keyboard.h"
Tower::Tower(TowerName t_TowerName, Vector2 t_Position)
{ 
	Name = t_TowerName;
	Id = static_cast<int>(t_TowerName);
	Position = t_Position;
}

void Tower::ChangePosition(Vector2 t_position)
{
	Position = t_position;
}

bool Tower::IsValidPath(short path)
{
	if (UpgradePath.size() == 2)
	{
		if (UpgradePath[path] && UpgradePath[path] < 5)
		{
			return true;
		}
		return false;
	}
	return true;
}

bool Tower::UpgradeTower(double money, short path)
{
	if (IsValidPath(path) && money >= TOWER_UPGRADE[Id][path][UpgradePath[path] + 1])
	{
		// use the , . / buttons to upgrade.
		
		INPUT input = Keyboard::keyPress(UPGRADE_KEY_CODE[path - 1]);
		Clock::wait(.1f);

		Keyboard::keyRelease(input);
		UpgradePath[path]++;
		return true;
	}
	return false;
}