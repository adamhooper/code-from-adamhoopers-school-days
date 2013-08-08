#ifndef NODE_H
#define NODE_H

#include "State.h"
#include "global.h"

/**
 * Stores a node in the search tree.
 *
 * Nodes use a simple minimax algorithm with alpha-beta pruning. The only
 * optimization of note: if our heuristic determines that a node has a value of
 * -0.5 or lower (on a scale from -1 to 1), in-depth scanning of that node is
 * abandoned.
 */
class Node
{
public:
	/**
	 * Constructs a search node.
	 *
	 * @param state State at this node.
	 * @param bool isWhite <code>true</code> if we're searching for the
	 *                     value from white's perspective.
	 */
	Node(const State& state, bool isWhite);

	/**
	 * Returns the worst-case maximum value of this node, using the minimax
	 * algorithm.
	 *
	 * @param alpha Alpha (in alpha-beta pruning algorithm).
	 * @param beta Beta (in alpha-beta pruning algorithm).
	 * @param depth Depth to search within this node.
	 */
	template<class Evaluator> float
		getMaxValue(float alpha, float beta, int depth) const;

	/**
	 * Returns the best-case minimum value of this node, using the minimax
	 * algorithm.
	 *
	 * @param alpha Alpha (in alpha-beta pruning algorithm).
	 * @param beta Beta (in alpha-beta pruning algorithm).
	 * @param depth Depth to search within this node.
	 */
	template<class Evaluator> float
		getMinValue(float alpha, float beta, int depth) const;

private:
	__pure inline float getLeafValue() const;

	State state;
	bool isWhite;
};

float
Node::getLeafValue() const
{
	Piece winner = this->state.winner();
	if (winner != PIECE_NONE)
	{
		if (winner == PIECE_WHITE)
		{
			return this->isWhite ? 1 : -1;
		}
		else
		{
			return this->isWhite ? -1 : 1;
		}
	}
	return 0;
}

#endif /* NODE_H */
