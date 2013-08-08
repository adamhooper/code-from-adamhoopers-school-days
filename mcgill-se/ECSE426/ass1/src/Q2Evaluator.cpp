#include "Q2Evaluator.h"

float
Q2Evaluator::evaluate(const Board& board)
{
	return (board.getNumPieces(true) - 5) * .2;
}
