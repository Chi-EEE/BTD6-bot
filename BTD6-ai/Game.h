#pragma once

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

#include "Memory.h"
#include "Window.h"
#include "Random.h"
#include "Mouse.h"
#include "Clock.h"
#include "Keyboard.h"
#include "Tower.h"
#include "Vector2.h"

class Game
{
public:
	void PlaceTower(Vector2 position, TowerName towerName, Vector2 offPosition);
	bool UpgradeTower(Tower tower, short path);

	void StartNextRound();

	void moveMouseToFarms();

	void AddTower(Tower tower);

	std::vector<Tower> getTowers() { return towers; }
private:
	Keyboard keyboard;
	Mouse mouse;
	Clock clock;

	std::vector<Tower> towers = {};
	std::vector<Vector2> farmPositions = {};
};

