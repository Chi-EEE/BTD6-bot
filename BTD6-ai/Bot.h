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
	void run(Game* game);
	bool BuyRandomTower(Game* game);
	bool SaveForRandomTower(Game* game);
	bool UpgradeRandomTower(Game* game);
	bool SaveForRandomUpgrade(Game* game);
private:
	Vector2 GetRandomPosition();
	bool GetUpgradeRound(Tower* tower, double health, double remainingMoney, int round, double upgradePrice, short path);
	Vector2 ClientPosition;
	Vector2 ClientSize;
	std::vector<UpgradeAction> upgradeActions;
	std::vector <BuildAction> buildActions;
	bool savingForPurchase = false;
	//std::vector<std::unique_ptr<Action>> previousActions;
	//std::vector<Action> actions;
};

