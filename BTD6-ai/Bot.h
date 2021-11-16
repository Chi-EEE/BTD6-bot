#pragma once
#include "Game.h"
#include "Memory.h"
#include "Vector2.h"
#include "Action.h"
#include "UpgradeAction.h"
#include "BuildAction.h"

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
	bool GetUpgradeRound(Tower* tower, double remainingMoney, int round, double upgradePrice, short path);
	Vector2 ClientPosition;
	Vector2 ClientSize;
	std::vector<UpgradeAction> upgradeActions;
	std::vector<std::unique_ptr<Action>> previousActions;
	//std::vector<Action> actions;
	bool savingForTower;
};

