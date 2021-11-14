#pragma once
#include "Action.h"
#include "ActionType.h"
#include "Tower.h"

class UpgradeAction : public Action
{
public:
	UpgradeAction(double t_round, Tower* t_tower, short t_path);

	Tower* getTower() { return tower; }
	short getPath() { return path; }
private:
	Tower* tower; // Tower to do action on
	short path;
};