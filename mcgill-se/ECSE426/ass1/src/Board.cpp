#include "Board.h"

#include "winning-bitsets.h"

/*
 * A Board is represented by two longs (>=32 bits), to save on space. These
 * represent arrays of bits. (The last five bits are the top row,
 * right-to-left; the next five bits are the second-to-top row, right-to-left;
 * etc.)
 *
 * hasPiece is set if a board position has a piece; pieceIsWhite is set if that
 * piece if white. (If the board has no piece in that particular position,
 * pieceIsWhite is ignored.)
 *
 * The simplest way to detect a pattern (e.g., a possible move or a winning
 * line) is using bitmasks; see the files in the <tt>helpers</tt> directory for
 * examples.
 */

static inline char
coordToOffset(const Coord& coord)
{
	return coord.getY() * 5 + coord.getX();
}

static inline bool
isCoordSet(const long& arr,
	   const Coord& coord)
{
	return 0 != (arr & (1 << coordToOffset(coord)));
}

static inline void
setCoord(long& arr,
	 const Coord& coord,
	 bool set)
{
	if (set)
	{
		arr |= (1 << coordToOffset(coord));
	}
	else
	{
		arr &= ~(1 << coordToOffset(coord));
	}
}

static Piece
oppositePiece(Piece piece)
{
	return piece == PIECE_WHITE ? PIECE_BLACK : PIECE_WHITE;
}

Board::Board()
	: hasPiece(0)
	, pieceIsWhite(0)
	, nPieces(0)
	, nWhite(0)
{
}

Board
Board::addPiece(const Coord& coord,
		bool isWhite) const
{
	Board copy(*this);

	setCoord(copy.hasPiece, coord, true);
	setCoord(copy.pieceIsWhite, coord, isWhite);

	copy.nPieces++;

	if (isWhite)
	{
		copy.nWhite++;
	}

	return copy;
}

Board
Board::applyMove(const Move& move) const
{
	Board copy(*this);

	const Coord orig(move.getCoord());

	setCoord(copy.hasPiece, orig, false);

	const bool white(isCoordSet(copy.pieceIsWhite, orig));

	Coord nextCoord(move.follow(orig));
	if (isCoordSet(copy.hasPiece, nextCoord))
	{
		setCoord(copy.pieceIsWhite, nextCoord, white);
		nextCoord = move.follow(nextCoord);

		copy.nWhite += (white ? 1 : -1);
	}

	setCoord(copy.hasPiece, nextCoord, true);
	setCoord(copy.pieceIsWhite, nextCoord, white);

	return copy;
}

int
Board::getNumPieces() const
{
	return this->nPieces;
}

int
Board::getNumPieces(bool isWhite) const
{
	return isWhite ? this->nWhite : (this->nPieces - this->nWhite);
}

Piece
Board::pieceAt(const Coord& coord) const
{
	if (isCoordSet(this->hasPiece, coord))
	{
		if (isCoordSet(this->pieceIsWhite, coord))
		{
			return PIECE_WHITE;
		}
		else
		{
			return PIECE_BLACK;
		}
	}

	return PIECE_NONE;
}

Piece
Board::winner(bool isWhite) const
{
	for (int i = 0; i < n_winning_bitsets; i++)
	{
		const long bitset = winning_bitsets[i];

		if ((this->hasPiece & bitset) != bitset) continue;

		if ((this->pieceIsWhite & bitset) == bitset)
		{
			return PIECE_WHITE;
		}
		if ((~this->pieceIsWhite & bitset) == bitset)
		{
			return PIECE_BLACK;
		}
	}

	if (!this->hasNextMove(isWhite))
	{
		return isWhite ? PIECE_BLACK : PIECE_WHITE;
	}

	return PIECE_NONE;
}

bool
Board::canMove(bool isWhite,
	       const Move& move) const
{
	const Coord coord(move.getCoord());

	Piece piece = isWhite ? PIECE_WHITE : PIECE_BLACK;
	if (piece != this->pieceAt(coord)) return false;

	const Coord adj1(move.follow(coord));
	if (!adj1.isValid()) return false;
	if (this->pieceAt(adj1) == PIECE_NONE) return true;

	const Coord adj2(move.follow(adj1));
	if (!adj2.isValid()) return false;
	return (this->pieceAt(adj1) == oppositePiece(piece)
		&& this->pieceAt(adj2) == PIECE_NONE);
}

List<Coord>
Board::getNextCoords() const
{
	List<Coord> ret;
	ret.ensureCapacity(25);

	for (char x = 0; x < 5; x++)
	{
		for (char y = 0; y < 5; y++)
		{
			Coord coord(x, y);
			if (!isCoordSet(this->hasPiece, coord))
			{
				ret.append(coord);
			}
		}
	}

	return ret;
}

List<const Move*>
Board::getNextMoves(bool isWhite) const
{
	List<const Move*> ret;

	const long empty = ~this->hasPiece;
	const long ours = isWhite ? this->pieceIsWhite : ~this->pieceIsWhite;
	const long theirs = isWhite ? ~this->pieceIsWhite : this->pieceIsWhite;

	for (int i = 0; i < n_move_bitsets; i++)
	{
		const move_bitset& mb = move_bitsets[i];

		if ((this->hasPiece & mb.full) != mb.full) continue;
		if ((empty & mb.empty) != mb.empty) continue;
		if ((ours & mb.ours) != mb.ours) continue;
		if ((theirs & mb.theirs) != mb.theirs) continue;

		ret.append(&mb.move);
	}

	return ret;
}

unsigned int
Board::hash() const
{
	return this->hasPiece << 7
		+ (this->hasPiece & this->pieceIsWhite);
}

bool
Board::operator==(const Board& rhs) const
{
	return (this->hasPiece == rhs.hasPiece
		&& ((this->hasPiece & this->pieceIsWhite)
		    == (rhs.hasPiece & rhs.pieceIsWhite)));
}

bool
Board::operator!=(const Board& rhs) const
{
	return this->operator==(rhs) == false;
}

std::ostream&
operator<<(std::ostream& os,
	   const Board& board)
{
	for (char offset = 0; offset < 25; offset++)
	{
		bool hasPiece = board.hasPiece & (1 << offset);
		bool pieceIsWhite = board.pieceIsWhite & (1 << offset);

		if (hasPiece)
		{
			os << (pieceIsWhite ? "1" : "0");
		}
		else
		{
			os << " ";
		}

		if (offset % 5 != 4)
		{
			os << ",";
		}
		else
		{
			os << std::endl;
		}
	}

	return os;
}

bool
Board::hasNextMove(bool isWhite) const
{
	if (this->getNumPieces() < 10) return true; // We can always add pieces

	// Return true if we have over 3 pieces
	int nMyPieces = this->getNumPieces(isWhite);

	if (nMyPieces > 3) return true;

	// Exactly 3 pieces --> there are only 4 possible no-move scenarios
	const long ours = this->hasPiece &
		(isWhite ? this->pieceIsWhite : ~this->pieceIsWhite);
	const long theirs = this->hasPiece &
		(isWhite ? ~this->pieceIsWhite : this->pieceIsWhite);

	if (nMyPieces == 3)
	{
		for (int i = 0; i < n_losing_3_bitsets; i++)
		{
			const losing_3_bitset& l3b = losing_3_bitsets[i];

			if ((theirs & l3b.winner) == l3b.winner
			    && (ours & l3b.loser) == l3b.loser)
			{
				return false;
			}
		}

		return true;
	}

	// Okay, we have 2 or 1 piece; do a full search
	// Transcribed from getNextMoves() (for speed)
	const long empty = ~this->hasPiece;

	for (int i = 0; i < n_move_bitsets; i++)
	{
		const move_bitset& mb = move_bitsets[i];

		if ((this->hasPiece & mb.full) != mb.full) continue;
		if ((empty & mb.empty) != mb.empty) continue;
		if ((ours & mb.ours) != mb.ours) continue;
		if ((theirs & mb.theirs) != mb.theirs) continue;

		return true;
	}

	return false;
}
