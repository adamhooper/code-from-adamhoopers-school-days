#ifndef AGENT_H
#define AGENT_H

#include "State.h"

/**
 * A player capable of generating moves.
 */
class Agent
{
public:
	/**
	 * Constructor.
	 *
	 * @param state Initial state.
	 * @param isWhite <code>true</code> if this agent is white.
	 */
	Agent(const State& state, bool isWhite = true);
	virtual ~Agent() {}

	/**
	 * Informs the Agent that the state has changed.
	 *
	 * @param state the new State.
	 */
	void setState(const State& state);

	/**
	 * Returns the Coord of a piece to add to the board.
	 *
	 * This Coord is what the Agent determines is the best possible Coord.
	 */
	virtual Coord getBestCoord() const = 0;

	/**
	 * Returns the Move to apply to the board.
	 *
	 * This Move is what the Agent determines is the best possible Move.
	 */
	virtual Move getBestMove() const = 0;

protected:
	State state;
	bool isWhite;
};

#endif /* AGENT_H */
