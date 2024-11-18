#include "Point.h"

using namespace Geometry;

Geometry::Point::Point() : mX(0), mY(0), mZ(0), mID(-1)
{
}

Geometry::Point::Point(double x, double y, double z, int id)
{
	mX = x;
	mY = y;
	mZ = z;
	mID = id;
}

Geometry::Point::~Point()
{
}

double Geometry::Point::getX() 
{
	return mX;
}

double Geometry::Point::getY() 
{
	return mY;
}

double Geometry::Point::getZ()  
{
	return mZ;
}

void Geometry::Point::setZ(double zVal)
{
	mZ = zVal;
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