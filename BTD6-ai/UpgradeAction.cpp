#include "UpgradeAction.h"

UpgradeAction::UpgradeAction(int t_round, short t_towerIndex, short t_path)
{
	round = t_round;
	towerIndex = t_towerIndex;
	path = t_path;
	type = ActionType::Upgrade;
}

UpgradeAction::UpgradeAction(UpgradeAction* upgradeAction)
{
	round = upgradeAction->getRound();
	towerIndex = upgradeAction->getTowerIndex();
	path = upgradeAction->getPath();
	type = ActionType::Upgrade;
}

json UpgradeAction::toJson()
{
	return json{ {"Type", 1}, {"Round", round}, {"TowerIndex", towerIndex}, {"Path", path} };
}