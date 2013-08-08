CXX = g++
CXXFLAGS = -Wall -Werror -O3 -funroll-loops

OBJS = \
	Agent.o		\
	AgentFactory.o	\
	Board.o		\
	ComputerAgent.o	\
	Coord.o		\
	HumanAgent.o	\
	Move.o		\
	Node.o		\
	Q1Evaluator.o	\
	Q2Evaluator.o	\
	Q2Evaluator2.o	\
	Q2Evaluator3.o

all : bw

winning-bitsets.h : ../helpers/gen-winning-bitsets.py
	python $< > $@
four-in-a-row-bitsets.h : ../helpers/gen-four-in-a-row-bitsets.py
	python $< > $@
three-in-a-row-bitsets.h : ../helpers/gen-three-in-a-row-bitsets.py
	python $< > $@
Agent.cpp : Agent.h Node.h
Agent.h : Set.h State.h
AgentFactory.cpp : AgentFactory.h ComputerAgent.h HumanAgent.h Q1Evaluator.h \
		   Q2Evaluator.h Q2Evaluator2.h Q2Evaluator3.h
AgentFactory.h :
Board.cpp : Board.h winning-bitsets.h
Board.h : Coord.h List.h Move.h Piece.h
ComputerAgent.cpp : ComputerAgent.h Node.h
ComputerAgent.h : Agent.h
Coord.cpp : Coord.h
HumanAgent.h : Agent.h
HumanAgent.cpp : HumanAgent.h
Move.cpp : Move.h
Move.h : Coord.h
Node.cpp : Node.h
Node.h : State.h global.h
Q1Evaluator.cpp : Q1Evaluator.h Board.h
Q2Evaluator.cpp : Q2Evaluator.h Board.h
Q2Evaluator2.cpp : Q2Evaluator2.h Board.h four-in-a-row-bitsets.h
Q2Evaluator3.cpp : Q2Evaluator2.h Board.h four-in-a-row-bitsets.h \
		   three-in-a-row-bitsets.h
Set.h : Hash.h
State.h : Board.h
main.cpp : AgentFactory.h Agent.h State.h

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

bw : $(OBJS) main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) winning-bitsets.h four-in-a-row-bitsets.h \
		three-in-a-row-bitsets.h bw
