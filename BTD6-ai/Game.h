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

	void GetMoney(double* variable);
	void GetHealth(double* variable);
	void GetRoundCount(int* variable);
	void GetTowerCount(int* variable);

	int MultiplyDefaultPrice(double defaultPrice);

	Tower GetRandomTower();
	Tower GetNextRandomTower(short indexCount);

	bool PlaceTower(TowerName TowerName, Vector2 Position);

	void StartNextRound();

	//void moveMouseToFarms();

	void AddTower(Tower tower);

	std::vector<Tower> getTowers() { return towers; }
private:

	HANDLE handle;
	DWORD64 processId;
	Memory memory = Memory{};

	void GetPositions();

	void GetMemoryAddresses();

	// Memory Addresses
	char* simulationAddress;
	char* moneyAddress;
	char* healthAddress;
	char* roundAddress;
	char* towerCountAddress;

	double money;
	double health;
	int roundCount;
	int towerCount;

	std::vector<Tower> towers = {};
	std::vector<short> randomTowerIndexes = {};
	std::vector<Vector2> farmPositions = {};

	Vector2 DeselectPosition;
};

