#ifndef COMPUTER_AGENT_H
#define COMPUTER_AGENT_H

#include "Agent.h"

/**
 * An Agent which uses the minimax algorithm to compute the best moves.
 *
 * The Evaluator template parameter lets us plug in new heuristic value
 * functions with no overhead. See AgentFactory for details.
 */
template<class Evaluator>
class ComputerAgent : public Agent
{
public:
	ComputerAgent(const State& state, bool isWhite);

	virtual Coord getBestCoord() const;
	virtual Move getBestMove() const;

private:
	bool calculateBestCoord(Coord& outCoord, int depth) const;
	bool calculateBestMove(Move& outMove, int depth) const;
};

#endif /* COMPUTER_AGENT_H */
