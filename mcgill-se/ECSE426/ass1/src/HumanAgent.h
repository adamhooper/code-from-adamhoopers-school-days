#ifndef HUMAN_AGENT_H
#define HUMAN_AGENT_H

#include "Agent.h"

/**
 * An agent which generates moves using human input (e.g., keyboard).
 *
 * getBestCoord() and getBestMove() expect input from the user in the form
 * <kbd>XY</kbd> and <kbd>XYD</kbd>, respectively.
 */
class HumanAgent : public Agent
{
public:
	HumanAgent(const State& state, bool isWhite);

	virtual Coord getBestCoord() const;
	virtual Move getBestMove() const;

private:
	const char *getCurrentPlayer() const;
};

#endif /* HUMAN_AGENT_H */
