#ifndef COORD_H
#define COORD_H

#include <iostream>

/**
 * A coordinate on the board.
 *
 * (1, 1) is the top-left corner.
 */
class Coord
{
public:
	/**
	 * Constructs a new Coord.
	 *
	 * @param x X-coordinate (left-to-right, left is 1).
	 * @param y Y-coordinate (top-to-bottom, top is 1).
	 */
	inline Coord(char x = -1, char y = -1) : x(x), y(y) {}

	/**
	 * Returns the X coordinate (left-to-right, left is 1).
	 */
	inline char getX() const { return this->x; }

	/**
	 * Returns the Y coordinate (top-to-bottom, top is 1).
	 */
	inline char getY() const { return this->y; }

	/**
	 * Returns <code>true</code> if this coordinate actually makes sense.
	 *
	 * A Coord which was input using operator>>(), for instance, may be
	 * invalid.
	 */
	inline bool isValid() const { return (this->x >= 0
					      && this->x <= 4
					      && this->y >= 0
					      && this->y <= 4); }

private:
	friend std::ostream& operator<<(std::ostream& os, const Coord& coord);
	friend std::istream& operator>>(std::istream& is, Coord& coord);

	char x;
	char y;
};

std::ostream& operator<<(std::ostream& os, const Coord& coord);
std::istream& operator>>(std::istream& is, Coord& coord);

#endif /* COORD_H */
