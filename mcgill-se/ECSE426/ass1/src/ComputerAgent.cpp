#include "ComputerAgent.h"

#include <sys/time.h>
#include <cstdlib>
#include <time.h>

#include "Node.h"

/**
 * We use iterative deepening to find the best move. Our time limit is seven
 * seconds, but we don't cancel out at that time. Instead, we check the time
 * after each iteration: if we've surpassed 750ms, we deem it too risky to
 * continue (this corresponds to an average fanout of 9.33 at the leaves, in
 * the worst case that the timer is checked at 749ms).
 */

template<class Evaluator>
ComputerAgent<Evaluator>::ComputerAgent(const State& state,
					bool isWhite)
	: Agent(state, isWhite)
{
}

template<class Evaluator>
Coord
ComputerAgent<Evaluator>::getBestCoord() const
{
	struct timeval tv1;
	gettimeofday(&tv1, 0);

	Coord ret;

	int depth = 0;
	int timeDiff = 0;
	while (true)
	{
		depth++;

		bool done = this->calculateBestCoord(ret, depth);
		if (done) break;

		struct timeval tv2;
		gettimeofday(&tv2, 0);

		timeDiff = (tv2.tv_sec - tv1.tv_sec) * 1000000
			+ tv2.tv_usec - tv1.tv_usec;

		if (timeDiff > 750000) break;
	}

	std::cout << "Move: " << ret << " (found with depth " << depth
		<< " in " << (timeDiff / 1000) << "ms)" << std::endl;

	return ret;
}

template<class Evaluator>
Move
ComputerAgent<Evaluator>::getBestMove() const
{
	struct timeval tv1;
	gettimeofday(&tv1, 0);

	Move ret;
	int depth = 0;
	int timeDiff = 0;
	while (true)
	{
		depth++;

		bool done = this->calculateBestMove(ret, depth);
		if (done) break;

		struct timeval tv2;
		gettimeofday(&tv2, 0);

		timeDiff = (tv2.tv_sec - tv1.tv_sec) * 1000000
			+ tv2.tv_usec - tv1.tv_usec;

		if (timeDiff > 750000) break;
	}

	std::cout << "Move: " << ret << " (found with depth " << depth
		<< " in " << (timeDiff / 1000) << "ms)" << std::endl;

	return ret;
}

template<class Evaluator>
bool
ComputerAgent<Evaluator>::calculateBestCoord(Coord& outCoord,
					     int depth) const
{
	List<Coord> coords(this->state.getNextCoords());

	float bestValue = -1.1; // below -1
	int nWithBestValue = 0;

	for (int i = 0; i < coords.getSize(); i++)
	{
		const Coord& coord(coords.at(i));
		State state(this->state.addPiece(coord));
		Node node(state, this->isWhite);

		float value = node.getMinValue<Evaluator>
			(-1 * std::numeric_limits<float>::max(),
			 std::numeric_limits<float>::max(),
			 depth - 1);

		if (value == bestValue)
		{
			nWithBestValue++;

			if (rand() < (float) RAND_MAX / nWithBestValue)
			{
				outCoord = coord;
			}
		}
		else if (value > bestValue)
		{
			outCoord = coord;

			if (value == 1) return true;

			bestValue = value;
			nWithBestValue = 1;
		}
	}

	return false;
}

template<class Evaluator>
bool
ComputerAgent<Evaluator>::calculateBestMove(Move& outMove,
					    int depth) const
{
	List<const Move*> moves(this->state.getNextMoves());

	float bestValue = -1.1; // below -1
	int nWithBestValue = 0;

	for (int i = 0; i < moves.getSize(); i++)
	{
		const Move& move(*moves.at(i));
		State state(this->state.applyMove(move));
		Node node(state, this->isWhite);

		float value = node.getMinValue<Evaluator>
			(-1 * std::numeric_limits<float>::max(),
			 std::numeric_limits<float>::max(),
			 depth - 1);

		if (value == bestValue)
		{
			nWithBestValue++;

			if (rand() < 1.0 / nWithBestValue * RAND_MAX)
			{
				outMove = move;
			}
		}
		else if (value > bestValue)
		{
			outMove = move;

			if (value == 1) return true;

			bestValue = value;
			nWithBestValue = 1;
		}
	}

	return false;
}

// Implementations

#include "Q1Evaluator.h"
template class ComputerAgent<Q1Evaluator>;
#include "Q2Evaluator.h"
template class ComputerAgent<Q2Evaluator>;
#include "Q2Evaluator2.h"
template class ComputerAgent<Q2Evaluator2>;
#include "Q2Evaluator3.h"
template class ComputerAgent<Q2Evaluator3>;
