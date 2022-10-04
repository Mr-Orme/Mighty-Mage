#include "Vector2D.h"
#include <fstream>

std::ostream& operator<<(std::ostream& os, const Vector2D& rhs)
{
	os << " " << rhs.x << " " << rhs.y;

	return os;
}


std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs)
{
	is >> lhs.x >> lhs.y;

	return is;
}

bool operator<(const Vector2D& srcL, const Vector2D& srcR)
{

	if (srcL.x == srcR.x)
	{
		return srcL.y < srcR.y;
	}
	return srcL.x < srcR.x;
}
