#include "UpgradeAction.h"

UpgradeAction::UpgradeAction(const int t_round, const short t_towerIndex, const short t_path)
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