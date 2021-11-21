#pragma once

#include <windows.h>
#include <iostream>
#include <vector>

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
	Game();

	double GetMoney();
	double GetHealth();
	double GetRoundCount();
	int GetTowerCount();

	int MultiplyDefaultPrice(double defaultPrice);

	void RandomizeTowers();

	Tower* GetTower(short towerIndex);

	short GetRandomTowerIndex(short indexCount);

	Tower* PlaceTower(TowerName TowerName, Vector2 Position);

    bool UpgradeTower(Tower* tower, short path);

	bool CanBuildTower(TowerName TowerName);

	void StartNextRound();

	void AddTower(Tower tower);

	Vector2 GetPlayPosition() { return playBoxPosition; }
	Vector2 GetPlaySize() { return playBoxSize; }
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
	double roundCount;
	int towerCount;

	Vector2 clientPosition;
	Vector2 clientSize;

	Vector2 playBoxPosition;
	Vector2 playBoxSize;

	Vector2 DebuildPosition;


	std::vector<Tower> towers = {};
	std::vector<short> randomTowerIndexes = {};
	bool HeroPlaced = false;
};

