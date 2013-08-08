#ifndef BOARD_H
#define BOARD_H

#include <iostream>

#include "Coord.h"
#include "List.h"
#include "Move.h"
#include "Piece.h"

/**
 * Stores a board position.
 *
 * After creation, Board instances are constant: no public methods will change
 * them. addPiece() and applyMove() will generate new boards with slight
 * changes; getNextCoords() and getNextMoves() will return lists of available
 * changes; and winner() will detect a winning position.
 */
class Board
{
public:
	/**
	 * Constructs an empty board.
	 *
	 * Nonempty boards can be generated <em>from</em> this board using
	 * addPiece() and applyMove().
	 */
	Board();

	/**
	 * Returns a copy of this board with a piece added.
	 *
	 * The coordinate is not checked for validity.
	 *
	 * @param coord Coordinate of the piece to add.
	 * @param isWhite <code>true</code> to add a white piece.
	 */
	Board addPiece(const Coord& coord, bool isWhite) const;

	/**
	 * Returns a copy of this board with a move applied.
	 *
	 * The player performing the move is inferred from the starting
	 * coordinate. The move is <em>not</em> checked for validity.
	 *
	 * @param move Move to perform.
	 */
	Board applyMove(const Move& move) const;

	/**
	 * Returns the number of pieces on the board.
	 */
	int getNumPieces() const;

	/**
	 * Returns the number of pieces belonging to the specified player.
	 *
	 * @param isWhite <code>true</code> to designate white.
	 */
	int getNumPieces(bool isWhite) const;

	/**
	 * Returns the piece at the given coordinate.
	 */
	Piece pieceAt(const Coord& coord) const;

	/**
	 * Returns the winner at the start of a turn.
	 *
	 * At the start of a turn, only the current player can have lost. In
	 * other words, if <code>isWhite = true</code>, then either white has
	 * just lost or nobody has.
	 *
	 * @param isWhite <code>true</code> if white is starting his turn.
	 */
	Piece winner(bool isWhite) const;

	/**
	 * Returns a list of all coordinates on which a piece can be placed.
	 */
	List<Coord> getNextCoords() const;

	/**
	 * Determines whether the specified player can perform the specified
	 * move.
	 *
	 * @param isWhite <code>true</code> to specify white.
	 * @param move Move to test for validity.
	 */
	bool canMove(bool isWhite, const Move& move) const;

	/**
	 * Returns a list of all moves available to the specified player.
	 *
	 * @param isWhite <code>true</code> to specify white.
	 */
	List<const Move*> getNextMoves(bool isWhite) const;

	unsigned int hash() const;
	bool operator==(const Board& rhs) const;
	bool operator!=(const Board& rhs) const;

private:
	bool hasNextMove(bool isWhite) const;

	friend std::ostream& operator<<(std::ostream& os, const Board& board);
	//friend std::istream& operator>>(std::istream& is, Board& board);

	long hasPiece;
	long pieceIsWhite;

	// Keep these handy to make Board::winner() faster
	int nPieces;
	int nWhite;

	// Give some evaluators direct access to the bitboards
	friend class Q2Evaluator2;
	friend class Q2Evaluator3;
};

std::ostream& operator<<(std::ostream& os, const Board& board);
//std::istream& operator>>(std::istream& is, Board& board);

#endif /* BOARD_H */
