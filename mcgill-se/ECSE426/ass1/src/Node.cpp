#include "Node.h"

#include <limits>

Node::Node(const State& state,
	   bool isWhite)
	: state(state)
	, isWhite(isWhite)
{
}

template<class Evaluator>
float
Node::getMaxValue(float alpha,
		  float beta,
		  int depth) const
{
	float leafValue = this->getLeafValue();
	if (leafValue != 0) return leafValue;

	if (depth == 0)
	{
		return (this->isWhite ? 1 : -1)
			* Evaluator::evaluate(this->state.board);
	}

	float maxVal = -1 * std::numeric_limits<float>::max();

	if (this->state.board.getNumPieces() < 10)
	{
		List<Coord> coords(this->state.getNextCoords());
		for (int i = 0; i < coords.getSize(); i++)
		{
			const Coord& coord(coords.at(i));
			Node node(this->state.addPiece(coord), this->isWhite);

			float nodeVal = node.getMinValue<Evaluator>
				(alpha, beta, depth - 1);

			maxVal = max(maxVal, nodeVal);

			if (maxVal >= beta) return maxVal;

			alpha = max(alpha, maxVal);
		}
	}
	else
	{
		List<const Move*> moves(this->state.getNextMoves());
		for (int i = 0; i < moves.getSize(); i++)
		{
			const Move& move(*moves.at(i));
			Node node(this->state.applyMove(move), this->isWhite);

			float nodeVal = node.getMinValue<Evaluator>
				(alpha, beta, depth - 1);

			maxVal = max(maxVal, nodeVal);

			if (maxVal >= beta) return maxVal;

			alpha = max(alpha, maxVal);
		}
	}

	return maxVal;
}

template<class Evaluator>
float
Node::getMinValue(float alpha,
		  float beta,
		  int depth) const
{
	float leafValue = this->getLeafValue();
	if (leafValue != 0) return leafValue;

	float estimate = (this->isWhite ? 1 : -1)
		* Evaluator::evaluate(this->state.board);

	if (depth == 0)
	{
		return estimate;
	}

	// Let's stop searching this path; we've lost anyway
	if (estimate < -0.5) return estimate;

	float minVal = std::numeric_limits<float>::max();

	if (this->state.board.getNumPieces() < 10)
	{
		List<Coord> coords(this->state.getNextCoords());
		for (int i = 0; i < coords.getSize(); i++)
		{
			const Coord& coord(coords.at(i));
			State state(this->state.addPiece(coord));

			Node node(state, this->isWhite);

			float nodeVal = node.getMaxValue<Evaluator>
				(alpha, beta, depth - 1);

			minVal = min(minVal, nodeVal);

			if (minVal <= alpha) return minVal;

			alpha = min(alpha, minVal);
		}
	}
	else
	{
		List<const Move*> moves(this->state.getNextMoves());
		for (int i = 0; i < moves.getSize(); i++)
		{
			const Move& move(*moves.at(i));
			State state(this->state.applyMove(move));

			Node node(state, this->isWhite);

			float nodeVal = node.getMaxValue<Evaluator>
				(alpha, beta, depth - 1);

			minVal = min(minVal, nodeVal);

			if (minVal <= alpha) return minVal;

			alpha = min(alpha, minVal);
		}
	}

	return minVal;
}

// Implementations

#include "Q1Evaluator.h"
template float Node::getMinValue<Q1Evaluator>(float, float, int) const;
template float Node::getMaxValue<Q1Evaluator>(float, float, int) const;
#include "Q2Evaluator.h"
template float Node::getMinValue<Q2Evaluator>(float, float, int) const;
template float Node::getMaxValue<Q2Evaluator>(float, float, int) const;
#include "Q2Evaluator2.h"
template float Node::getMinValue<Q2Evaluator2>(float, float, int) const;
template float Node::getMaxValue<Q2Evaluator2>(float, float, int) const;
#include "Q2Evaluator3.h"
template float Node::getMinValue<Q2Evaluator3>(float, float, int) const;
template float Node::getMaxValue<Q2Evaluator3>(float, float, int) const;
