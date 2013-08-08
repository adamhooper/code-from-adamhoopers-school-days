#include "Move.h"

std::ostream&
operator<<(std::ostream& os,
	   const Move& move)
{
	// Format: (3, 2) moving North is "32N"

	os << move.getCoord();

	switch (move.dir)
	{
		case Move::DIR_N:
			os << "N";
			break;
		case Move::DIR_E:
			os << "E";
			break;
		case Move::DIR_S:
			os << "S";
			break;
		case Move::DIR_W:
			os << "W";
			break;
	}

	return os;
}

std::istream&
operator>>(std::istream& is,
	   Move& move)
{
	is >> move.coord;

	char dir;
	is >> dir;

	switch (dir)
	{
		case 'N':
			move.dir = Move::DIR_N;
			break;
		case 'E':
			move.dir = Move::DIR_E;
			break;
		case 'S':
			move.dir = Move::DIR_S;
			break;
		case 'W':
			move.dir = Move::DIR_W;
			break;
		default:
			// Invalidate move
			move.coord = Coord();
			move.dir = Move::DIR_N;
	}

	return is;
}

