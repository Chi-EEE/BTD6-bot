#pragma once
#include "Action.h"
#include "ActionType.h"

#include "TowerName.h"
#include "Vector2.h"

class BuildAction: public Action
{
public:
	BuildAction(BuildAction* buildAction);
	BuildAction(int t_round, TowerName t_towerName);
	BuildAction(int t_round, TowerName t_towerName, Vector2 t_position);

    json toJson();

	TowerName getTowerName() { return towerName; }

	Vector2 getPosition() { return Position; }
	void setPosition(Vector2 t_position) { Position = t_position; }
private:
	TowerName towerName; // Tower to build
	Vector2 Position;
};