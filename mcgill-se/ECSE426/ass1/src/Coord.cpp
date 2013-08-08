#include "Coord.h"

std::ostream&
operator<<(std::ostream& os,
	   const Coord& coord)
{
	// Format: "(2, 2)" is output as "33"
	return os << (coord.x + 1) << (coord.y + 1);
}

std::istream&
operator>>(std::istream& is,
	   Coord& coord)
{
	is >> coord.x;
	coord.x -= '1';

	is >> coord.y;
	coord.y -= '1';

	return is;
}
