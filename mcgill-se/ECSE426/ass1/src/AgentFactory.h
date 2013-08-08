#ifndef AGENT_FACTORY_H
#define AGENT_FACTORY_H

#include <iostream>
#include <string>

class Agent;
class State;

/**
 * Generates an agent based on a string.
 *
 * See the printDescriptions() implementation for a list of possible agents.
 */
class AgentFactory
{
public:
	/**
	 * Returns a new agent.
	 *
	 * @param type Type of agent.
	 * @param state Initial state of the agent.
	 * @param isWhite <code>true</code> if the agent is white.
	 */
	static Agent *generateAgent(const std::string& type,
				    const State& state, bool isWhite);

	/**
	 * Prints all available agent types to the given output stream.
	 *
	 * @param os Output stream.
	 */
	static void printDescriptions(std::ostream& os);
};

#endif /* AGENT_FACTORY_H */
