#ifndef STATE_H
#define STATE_H

#include "Board.h"

/**
 * Represents a game state: a Board and the current player.
 */
class State
{
public:
	/**
	 * Constructs a new State.
	 *
	 * @param board Board position.
	 * @param whiteTurn <code>true</code> if it is white's turn.
	 */
	State(const Board& board = Board(), bool whiteTurn = true)
		: board(board), whiteTurn(whiteTurn) {}

	/**
	 * Returns a new State with a peice added and turns switched.
	 *
	 * @param coord Coordinate at which to add a piece.
	 * @see Board::addPiece()
	 */
	inline State addPiece(const Coord& coord) const;

	/**
	 * Returns a new State with a move applied and turns switched.
	 *
	 * @param move Move to apply.
	 * @see Board::applyMove()
	 */
	inline State applyMove(const Move& move) const;

	/**
	 * Returns a list of all coordinates at which a piece can be added.
	 *
	 * @see Board::getNextCoords()
	 */
	inline List<Coord> getNextCoords() const;

	/**
	 * Returns a list of all moves which can be applied.
	 *
	 * @see Board::getNextMoves()
	 */
	inline List<const Move*> getNextMoves() const;

	/**
	 * Returns the winner, if there is one.
	 *
	 * @see Board::winner()
	 */
	inline Piece winner() const;

	inline unsigned int hash() const;
	inline bool operator==(const State& rhs) const;

	Board board;	///< The current board position.
	bool whiteTurn;	///< <code>true</code> if it is white's turn.
};

State
State::addPiece(const Coord& coord) const
{
	return State(this->board.addPiece(coord, this->whiteTurn),
		     !this->whiteTurn);
}

State
State::applyMove(const Move& move) const
{
	return State(this->board.applyMove(move), !this->whiteTurn);
}

List<Coord>
State::getNextCoords() const
{
	return this->board.getNextCoords();
}

List<const Move*>
State::getNextMoves() const
{
	return this->board.getNextMoves(this->whiteTurn);
}

Piece
State::winner() const
{
	return this->board.winner(this->whiteTurn);
}

unsigned int
State::hash() const
{
	return this->board.hash() + (this->whiteTurn ? 1 : 0);
}

bool
State::operator==(const State& rhs) const
{
	return this->board == rhs.board && this->whiteTurn == rhs.whiteTurn;
}

#endif /* STATE_H */
