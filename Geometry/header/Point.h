#pragma once
#include <vector>

namespace Geometry
{
    class Point
    {
    private:
        double mX;
        double mY;
        double mZ;
        int mID;
        static int sNextID;

    public:
        Point();
        Point(double x, double y, double z);
        ~Point();
        double getZ();
        int getId();
        void setZ(double zVal);
        std::vector<double> getCoords();
        bool operator<(const Point& other);
    };
}