#include "AgentFactory.h"

#include "ComputerAgent.h"
#include "HumanAgent.h"

#include "Q1Evaluator.h"
#include "Q2Evaluator.h"
#include "Q2Evaluator2.h"
#include "Q2Evaluator3.h"

Agent *
AgentFactory::generateAgent(const std::string& type,
			    const State& state,
			    bool isWhite)
{
	if (type == "human")
	{
		return new HumanAgent(state, isWhite);
	}

	if (type == "q1")
	{
		return new ComputerAgent<Q1Evaluator>(state, isWhite);
	}

	if (type == "q2-1")
	{
		return new ComputerAgent<Q2Evaluator>(state, isWhite);
	}

	if (type == "q2-2")
	{
		return new ComputerAgent<Q2Evaluator2>(state, isWhite);
	}

	if (type == "q2-3")
	{
		return new ComputerAgent<Q2Evaluator3>(state, isWhite);
	}

	return 0;
}

void
AgentFactory::printDescriptions(std::ostream& os)
{
	os << "\thuman - a human player using the keyboard" << std::endl;
	os << "\tq1 - simple heuristic (1 for win, -1 for loss)" << std::endl;
	os << "\tq2-1 - aims to gain pieces" << std::endl;
	os << "\tq2-2 - aims to gain pieces and 4-in-a-rows" << std::endl;
	os << "\tq2-3 - aims to gain pieces and 3-in-a-rows" << std::endl;
}
