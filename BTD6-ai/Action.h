#pragma once
class Action
{
public:
	ActionType getActionType() { return actionType; }
	bool isReady(double t_currentRound) { return t_currentRound >= round; }
protected:
	ActionType actionType;
	double round; // Round to do action on
};

