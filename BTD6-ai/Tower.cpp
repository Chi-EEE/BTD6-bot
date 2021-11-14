#include "Tower.h"

#include "Globals.h"
#include "Clock.h"
#include "Mouse.h"
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
	size_t NumberOfElements = sizeof(UpgradePath) / sizeof(UpgradePath[0]);
	std::cout << "COunt: " << NumberOfElements << "\n"; // TOOOOOOOOOOOOOOOOOOOO FIX THE UPGRADE PATHS
	if (NumberOfElements == 2)
	{
		if (UpgradePath[path] && UpgradePath[path] < 5)
		{
			return true;
		}
		return false;
	}
	return true;
}