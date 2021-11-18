#pragma once
#include "Action.h"
#include "ActionType.h"
#include "TowerName.h"

class BuildAction: public Action
{
public:
	BuildAction(int t_round, TowerName t_towerName);

	TowerName getTowerName() { return towerName; }
private:
	TowerName towerName; // Tower to build
};