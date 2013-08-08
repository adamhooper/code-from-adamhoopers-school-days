#ifndef Q2_EVALUATOR_2_H
#define Q2_EVALUATOR_2_H

#include "Board.h"

/**
 * Estimates the value of a board position for white (from -1.0 to 1.0).
 *
 * The value is gotten as follows: (numPieces - 5) * 0.18 + [bonus], where
 * [bonus] is 0.1 if a four-in-a-row position exists, and -0.1 if a
 * four-in-a-row position exists for the opponent.
 *
 * Intuitively, this should help us get into inevitable-win positions (and
 * avoid inevitable-loss positions) without having to look ahead to the actual
 * win. Piece value is still more important than position, though (you can't
 * win with 4 pieces).
 */
class Q2Evaluator2
{
public:
	static float evaluate(const Board& board);
};

#endif /* Q2_EVALUATOR_2_H */
