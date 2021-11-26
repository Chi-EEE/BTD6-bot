#pragma once
#include "Action.h"
#include "ActionType.h"

class UpgradeAction : public Action
{
public:
	UpgradeAction(int t_round, short t_towerIndex, short t_path);

	UpgradeAction(UpgradeAction* upgradeAction);

	json toJson();

	short getTowerIndex() { return towerIndex; }
	short getPath() { return path; }
private:
	short towerIndex;
	short path;
};