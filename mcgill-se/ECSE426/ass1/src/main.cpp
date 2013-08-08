#include <iostream>
#include <memory>

#include "Agent.h"
#include "AgentFactory.h"
#include "State.h"

static void
printUsage(const char *me)
{
	std::cerr << "Usage: " << me << " [white player] [black player]"
		<< std::endl;

	std::cerr << "Where [player] is one of:" << std::endl;
	AgentFactory::printDescriptions(std::cerr);
}

int
main(int argc,
     char *argv[])
{
	State state;
	bool isWhite = true;

	if (argc != 3)
	{
		printUsage(argv[0]);
		return 1;
	}

	std::auto_ptr<Agent> whiteAgent
		(AgentFactory::generateAgent(argv[1], state, true));
	if (whiteAgent.get() == NULL)
	{
		std::cerr << "Invalid agent name '" << argv[1] << "'"
			<< std::endl;
		printUsage(argv[0]);
		return 1;
	}

	std::auto_ptr<Agent> blackAgent
		(AgentFactory::generateAgent(argv[2], state, false));
	if (blackAgent.get() == NULL)
	{
		std::cerr << "Invalid agent name '" << argv[1] << "'"
			<< std::endl;
		printUsage(argv[0]);
		return 1;
	}

	for (int i = 0; i < 10 && state.winner() == PIECE_NONE; i++)
	{
		std::cout << std::endl << state.board << std::endl;

		whiteAgent->setState(state);
		blackAgent->setState(state);

		Coord bestCoord;

		if (isWhite)
		{
			bestCoord = whiteAgent->getBestCoord();
		}
		else
		{
			bestCoord = blackAgent->getBestCoord();
		}

		state = state.addPiece(bestCoord);

		isWhite = !isWhite;
	}

	while (state.winner() == PIECE_NONE)
	{
		std::cout << std::endl << state.board << std::endl;

		whiteAgent->setState(state);
		blackAgent->setState(state);

		Move bestMove;

		if (isWhite)
		{
			bestMove = whiteAgent->getBestMove();
		}
		else
		{
			bestMove = blackAgent->getBestMove();
		}

		state = state.applyMove(bestMove);

		isWhite = !isWhite;
	}

	std::cout << "Done! Winner is " << (isWhite ? "black" : "white")
		<< "." << std::endl;

	return 0;
}
