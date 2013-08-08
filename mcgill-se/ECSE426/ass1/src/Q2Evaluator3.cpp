#include "Q2Evaluator3.h"

#include "four-in-a-row-bitsets.h"
#include "three-in-a-row-bitsets.h"

float
Q2Evaluator3::evaluate(const Board& board)
{
	float base = (board.getNumPieces(true) - 5) * .18;

	float bonus = 0;

	const long ours = board.hasPiece & board.pieceIsWhite;
	const long theirs = board.hasPiece & ~board.pieceIsWhite;

	for (int i = 0; i < n_four_in_a_row_bitsets; i++)
	{
		const long bitset = four_in_a_row_bitsets[i];

		if ((ours & bitset) == bitset)
		{
			bonus += 0.1;
			break;
		}
	}

	for (int i = 0; i < n_four_in_a_row_bitsets; i++)
	{
		const long bitset = four_in_a_row_bitsets[i];

		if ((theirs & bitset) == bitset)
		{
			bonus -= 0.1;
			break;
		}
	}

	if (bonus != 0) return base + bonus;

	for (int i = 0; i < n_three_in_a_row_bitsets; i++)
	{
		const long bitset = three_in_a_row_bitsets[i];

		if ((ours & bitset) == bitset)
		{
			bonus += 0.03;
		}
	}

	for (int i = 0; i < n_three_in_a_row_bitsets; i++)
	{
		const long bitset = three_in_a_row_bitsets[i];

		if ((theirs & bitset) == bitset)
		{
			bonus -= 0.03;
		}
	}

	return base + bonus;
}
