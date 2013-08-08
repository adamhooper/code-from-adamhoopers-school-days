#include "HumanAgent.h"

HumanAgent::HumanAgent(const State& state,
		       bool isWhite)
	: Agent(state, isWhite)
{
}

Coord
HumanAgent::getBestCoord() const
{
	Coord coord;

	while ((std::cout << "Enter a position for a "
		<< this->getCurrentPlayer() << " piece: ")
	       && (!(std::cin >> coord)
		   || !coord.isValid()
		   || this->state.board.pieceAt(coord) != PIECE_NONE))
	{
		std::cout << "That's an invalid position; ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
				'\n');

		if (std::cin.eof())
		{
			throw std::exception();
		}
	}

	return coord;
}

Move
HumanAgent::getBestMove() const
{
	Move move;

	while ((std::cout << "Enter a move for " << this->getCurrentPlayer()
		<< ": ")
	       && (!(std::cin >> move)
		   || !move.isValid()
		   || !this->state.board.canMove(this->isWhite, move)))
	{
		std::cout << "That's an invalid move; ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
				'\n');

		if (std::cin.eof())
		{
			throw std::exception();
		}
	}

	return move;
}

const char *
HumanAgent::getCurrentPlayer() const
{
	return this->isWhite ? "white" : "black";
}
