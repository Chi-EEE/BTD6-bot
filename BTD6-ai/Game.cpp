#include "Game.h"
#include "Globals.h"

bool Game::PlaceTower(Vector2 position, int towerIndex)
{
	int newtowerCount = 0;
	INPUT input = keyboard.keyPress(TOWER_SCAN_CODE[towerIndex]);

	mouse.setPosition(position);
	mouse.leftMouseDown();

	clock.wait(.1f);
	keyboard.keyRelease(input);
	mouse.leftMouseUp();
	mouse.setPosition(position);
	// Reads memory
	if (Tower::getTowerCount() > newtowerCount)
	{
		Tower newTower = Tower{ towerIndex, TOWER_NAME[towerIndex], position };
		towers.push_back(newTower);
		return true;
	}
	return false;
}

bool Game::UpgradeTower(int towerVectorIndex, short path)
{
	int money = 0;

	if (money >= TOWER_UPGRADE[towers[towerVectorIndex].getTowerIndex()][path][towers[towerVectorIndex].getLatestUpgradePath(path) + 1])
	{
		// use the , . / buttons to upgrade.
		towers[towerVectorIndex].UpgradePath(path);
		return true;
	}
	return false;
}

void Game::moveMouseToFarms()
{ // Rework to move between farms to avoid bananas from being removed
	int X_Total = 0;
	int Y_Total = 0;

	if (farmPositions.size() > 0)
	{
		for (Vector2 position : farmPositions)
		{
			X_Total += position.X;
			Y_Total += position.Y;
		}
		X_Total /= farmPositions.size();
		Y_Total /= farmPositions.size();
		mouse.setPosition(X_Total, Y_Total);
	}
}