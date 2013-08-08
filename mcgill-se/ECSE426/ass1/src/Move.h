#ifndef MOVE_H
#define MOVE_H

#include <iostream>

#include "Coord.h"

/**
 * Represents a move: a starting coordinate and a direction.
 */
class Move
{
public:
	/**
	 * Enumerates available directions.
	 */
	enum Dir
	{
		DIR_N, ///< North
		DIR_E, ///< East
		DIR_S, ///< South
		DIR_W, ///< West
	};

	/**
	 * Constructs a new Move.
	 *
	 * @param coord Starting coordinate.
	 * @param dir Direction.
	 */
	Move(const Coord& coord = Coord(), Dir dir = DIR_N)
		: coord(coord), dir(dir) {}

	/**
	 * Returns the starting coordinate.
	 */
	inline Coord getCoord() const { return this->coord; }

	/**
	 * Returns the direction.
	 */
	inline Dir getDir() const { return this->dir; }

	/**
	 * Returns <code>true</code> if this move makes sense.
	 *
	 * A <code>true</code> value does not indicate that a move can actually
	 * be applied to a board (use Board::canMove() for that). A
	 * <code>true</code> value only means that the coordinate and direction
	 * are sane. (isValid() may return <code>false</code> if operator<<()
	 * failed, for instance.
	 */
	inline bool isValid() const;

	/**
	 * Returns the Coord next to the specified one, following our
	 * direction.
	 *
	 * (This is an implementation detail: sometimes a Move moves one square
	 * and sometimes it moves two; we use the parameter to use the same
	 * direction for both coordinates.)
	 *
	 * @param coord Coordinate from which to begin.
	 */
	inline Coord follow(const Coord& coord) const;

private:
	friend std::ostream& operator<<(std::ostream& os, const Move& move);
	friend std::istream& operator>>(std::istream& is, Move& move);

	Coord coord;
	Dir dir;
};

bool
Move::isValid() const
{
	return (this->coord.isValid()
		&& ((this->dir == DIR_N && this->coord.getY() > 0)
		    || (this->dir == DIR_E && this->coord.getX() < 4)
		    || (this->dir == DIR_S && this->coord.getY() < 4)
		    || (this->dir == DIR_W && this->coord.getX() > 0)));
}

Coord
Move::follow(const Coord& coord) const
{
	switch (this->dir)
	{
		case DIR_N:
			return Coord(coord.getX(), coord.getY() - 1);
		case DIR_E:
			return Coord(coord.getX() + 1, coord.getY());
		case DIR_S:
			return Coord(coord.getX(), coord.getY() + 1);
		case DIR_W:
			return Coord(coord.getX() - 1, coord.getY());
	}

	return Coord();
}

std::ostream& operator<<(std::ostream& os, const Move& move);
std::istream& operator>>(std::istream& is, Move& move);

#endif /* MOVE_H */
