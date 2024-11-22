#include "Point.h"

namespace Geometry
{
    int Point::sNextID = 0;

    Point::Point() : mX(0), mY(0), mZ(0), mID(sNextID++)
    {
    }

    Point::Point(double x, double y, double z) : mX(x), mY(y), mZ(z), mID(sNextID++)
    {
    }

    Point::~Point()
    {
    }

    double Point::getZ()
    {
        return mZ;
    }

    int Point::getId()
    {
        return mID;
    }

    void Point::setZ(double zVal)
    {
        mZ = zVal;
    }

    std::vector<double> Point::getCoords()
    {
        return { mX, mY, mZ };
    }

    bool Point::operator<(const Point& other)
    {
        return mID < other.mID;
    }
}