#pragma once
#include "ActionType.h"
class Action
{
public:
	bool isReady(int t_currentRound) { return t_currentRound >= round; }
	ActionType getType() { return type; }
protected:
	int round; // Round to do action on
	ActionType type;
};

