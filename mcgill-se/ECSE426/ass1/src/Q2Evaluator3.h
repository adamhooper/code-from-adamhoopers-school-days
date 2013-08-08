#ifndef Q2_EVALUATOR_3_H
#define Q2_EVALUATOR_3_H

#include "Board.h"

/**
 * Estimates the value of a board position for white (from -1.0 to 1.0).
 *
 * The value is the same as in Q2Evaluator2, but if there are no four-in-a-row
 * positions, the bonus is 0.03 for any three-in-a-row positions (and -0.03 if
 * the opponent has them).
 */
class Q2Evaluator3
{
public:
	static float evaluate(const Board& board);
};

#endif /* Q2_EVALUATOR_3_H */
