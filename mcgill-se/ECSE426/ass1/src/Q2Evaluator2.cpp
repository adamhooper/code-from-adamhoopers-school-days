#include "Q2Evaluator2.h"

#include "four-in-a-row-bitsets.h"

float
Q2Evaluator2::evaluate(const Board& board)
{
	float base = (board.getNumPieces(true) - 5) * .18;

	float bonus = 0;

	const long ours = board.hasPiece & board.pieceIsWhite;

	for (int i = 0; i < n_four_in_a_row_bitsets; i++)
	{
		const long bitset = four_in_a_row_bitsets[i];

		if ((ours & bitset) == bitset)
		{
			bonus += 0.1;
			break;
		}
	}

	const long theirs = board.hasPiece & ~board.pieceIsWhite;

	for (int i = 0; i < n_four_in_a_row_bitsets; i++)
	{
		const long bitset = four_in_a_row_bitsets[i];

		if ((theirs & bitset) == bitset)
		{
			bonus -= 0.1;
			break;
		}
	}

	return base + bonus;
}
