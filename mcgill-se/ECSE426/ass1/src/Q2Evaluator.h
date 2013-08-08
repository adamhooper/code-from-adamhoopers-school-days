#ifndef Q2_EVALUATOR_H
#define Q2_EVALUATOR_H

#include "Board.h"

/**
 * Estimates the value of a board position for white (from -1.0 to 1.0).
 *
 * This algorithm simply aims for material advantage: (numPieces - 5) * 0.2.
 */
class Q2Evaluator
{
public:
	static float evaluate(const Board& board);
};

#endif /* Q2_EVALUATOR_H */
