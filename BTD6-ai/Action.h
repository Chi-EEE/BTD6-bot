#pragma once
class Action
{
public:
	bool isReady(double t_currentRound) { return t_currentRound >= round; }
protected:
	double round; // Round to do action on
};

