#pragma once
#include "Action.h"
#include "ActionType.h"
#include "Tower.h"

class UpgradeAction : public Action
{
public:
	UpgradeAction(double t_round, Tower* tower);

	Tower* getTower() { return tower; }
private:
	Tower* tower; // Tower to do action on
};