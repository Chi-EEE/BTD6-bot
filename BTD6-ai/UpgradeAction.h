#pragma once
#include "Action.h"
#include "ActionType.h"
#include "Tower.h"

class UpgradeAction : public Action
{
public:
	UpgradeAction(int t_round, short t_towerIndex, short t_path);

	short getTowerIndex() { return towerIndex; }
	short getPath() { return path; }
private:
	short towerIndex;
	short path;
};