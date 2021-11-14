#pragma once
#include "Action.h"
#include "ActionType.h"
#include "TowerName.h"

class BuildAction: public Action
{
	BuildAction(double t_round, TowerName tower);

	TowerName getTowerName() { return towerName; }
private:
	TowerName towerName; // Tower to build
};