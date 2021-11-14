#include "UpgradeAction.h"

UpgradeAction::UpgradeAction(double t_round, Tower* t_tower)
{
	actionType = ActionType::Upgrade;
	round = t_round;
	tower = t_tower;
}
