#ifndef Q1_EVALUATOR_H
#define Q1_EVALUATOR_H

#include "Board.h"

/**
 * Estimates the value of a board position for white (from -1.0 to 1.0).
 *
 * This evaluator simply returns 0.0, since if it is being called it's because
 * we haven't found a terminal node.
 */
class Q1Evaluator
{
public:
	static float evaluate(const Board& board);
};

#endif /* Q1_EVALUATOR_H */
