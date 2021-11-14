#pragma once
#include "Game.h"
#include "Memory.h"
#include "Vector2.h"

class Bot
{
public:
    Bot(Game* game);
	void BuyRandomTower(Game* game);
	void UpgradeRandomTower(Game* game);
	void SaveForRandomUpgrade(Game* game);
	void run(Game* game);
private:
	Vector2 GetRandomPosition();

	Vector2 ClientPosition;
	Vector2 ClientSize;

	//bool savingForTower;
};

