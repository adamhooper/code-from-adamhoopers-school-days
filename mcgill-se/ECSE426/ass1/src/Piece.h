#ifndef PIECE_H
#define PIECE_H

/**
 * Tristate representing a piece.
 *
 * This is used, for instance, as return value to Board::pieceAt().
 */
enum Piece
{
	PIECE_WHITE,	///< White
	PIECE_BLACK,	///< Black
	PIECE_NONE,	///< No piece
};

#endif /* PIECE_H */
