#include "Agent.h"

Agent::Agent(const State& state,
	     bool isWhite)
	: state(state)
	, isWhite(isWhite)
{
	srand(time(0));
}

void
Agent::setState(const State& state)
{
	this->state = state;
}
