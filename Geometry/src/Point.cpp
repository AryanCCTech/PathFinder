#include "Point.h"

using namespace Geometry;

Geometry::Point::Point(double x, double y, double z)
{
	mX = x;
	mY = y;
	mZ = z;
}

Geometry::Point::~Point()
{
}

std::vector<double> Geometry::Point::coord()
{
	std::vector<double> coords(3);
	coords.push_back(mX);
	coords.push_back(mY);
	coords.push_back(mZ);
	return coords;
}

bool Geometry::Point::operator<(const Point& other)
{
	if (mX != other.mX)
	{
		return mX < other.mX;
	}
	if (mY != other.mY)
	{
		return mY < other.mY;
	}
	return mZ < other.mZ;
}