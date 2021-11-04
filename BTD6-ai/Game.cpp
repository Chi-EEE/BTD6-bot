#include "Game.h"
#include "Globals.h"

bool Game::PlaceTower(Vector2 position, TowerName towerName, Vector2 offPosition)
{
	int newtowerCount = 0;
	INPUT input = keyboard.keyPress(TOWER_SCAN_CODE[static_cast<int>(towerName)]);

	mouse.setPosition(position);
	mouse.leftMouseDown();

	clock.wait(.1f);
	keyboard.keyRelease(input);
	mouse.leftMouseUp();
	mouse.setPosition(position);
	// Reads memory
	if (Tower::getTowerCount() > newtowerCount)
	{
		Tower newTower = Tower{ towerName, position };
		towers.push_back(newTower);
		return true;
	}
	return false;
}

bool Game::UpgradeTower(Tower tower, short path)
{
	int money = 0;

	if (money >= TOWER_UPGRADE[static_cast<int>(tower.getTowerName())][path][tower.getLatestUpgradePath(path) + 1])
	{
		// use the , . / buttons to upgrade.
		
		INPUT input = keyboard.keyPress(UPGRADE_KEY_CODE[path - 1]);
		clock.wait(.1f);

		keyboard.keyRelease(input);
		tower.UpgradePath(path);
		return true;
	}
	return false;
}

void Game::StartNextRound()
{
	INPUT input = keyboard.keyPress(SPACE_SCAN_CODE);
	clock.wait(.1f);
	keyboard.keyRelease(input);
}

void Game::moveMouseToFarms()
{ 
	for (Vector2 position : farmPositions)
	{
		clock.wait(.1f);
		mouse.setPosition(position);
	}
}