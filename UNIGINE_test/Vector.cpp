#include "Vector.h"

using namespace ut;

Vector ut::operator+(const Vector& left, const Vector& right)
{
	return { left.x + right.x, left.y + right.y };
}

Vector ut::operator-(const Vector& left, const Vector& right)
{
	return { left.x - right.x, left.y - right.y };
}

std::ostream& ut::operator<<(std::ostream& ostream, const Vector& vector)
{
	return ostream << "(" << vector.x << "," << vector.y << ")";
}