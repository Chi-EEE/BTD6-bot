#pragma once
#include <queue>
#include "json.hpp"

#include "Game.h"
#include "Memory.h"
#include "Vector2.h"
#include "Action.h"
#include "UpgradeAction.h"
#include "BuildAction.h"

using json = nlohmann::json;
class Bot
{
public:
    Bot(Game* game);
	void run(Game* game);
	bool BuyHero(Game* game);
	bool BuyRandomTower(Game* game);
	bool SaveForRandomTower(Game* game);
	bool UpgradeRandomTower(Game* game);
	bool SaveForRandomUpgrade(Game* game);

	void Save();
private:
	Vector2 GetRandomPosition();
	bool GetUpgradeRound(Game* game, Tower* tower, const short towerIndex, const double upgradePrice, const short path);
	Vector2 ClientPosition;
	Vector2 ClientSize;
	std::queue <Action*> actions;
	std::vector <Action*> previousActions;
};

