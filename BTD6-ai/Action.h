#pragma once
#include "ActionType.h"
#include "json.hpp"
using json = nlohmann::json;

class Action
{
public:
	int getRound() { return round; }
	ActionType getType() { return type; }
	virtual json toJson() = 0;
protected:
	int round; // Round to do action on
	ActionType type;
};

