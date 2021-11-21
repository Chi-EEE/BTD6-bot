#include "UpgradeAction.h"

UpgradeAction::UpgradeAction(int t_round, short t_towerIndex, short t_path)
{
	round = t_round;
	towerIndex = t_towerIndex;
	path = t_path;
	type = ActionType::Upgrade;
}
