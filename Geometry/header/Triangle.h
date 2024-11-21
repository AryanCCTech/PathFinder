#pragma once
#include "point.h"

class STLFileReader;

namespace Geometry
{
    class Triangle
    {
    private:

        Point p1;
        Point p2;
        Point p3;
        Point normal;
    public:
        Triangle();
        Triangle(Point normal, Point p1, Point p2, Point p3);
        ~Triangle();

        Point& P1();
        Point& P2();
        Point& P3();
        Point& Normal();
        std::vector<Point> getPoints();

        void setP1(Geometry::Point&);
        void setP2(Geometry::Point&);
        void setP3(Geometry::Point&);
    };
}