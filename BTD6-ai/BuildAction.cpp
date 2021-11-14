#include "BuildAction.h"

BuildAction::BuildAction(double t_round, TowerName tower)
{
	actionType = ActionType::Build;
	round = t_round;
	tower = tower;
}
