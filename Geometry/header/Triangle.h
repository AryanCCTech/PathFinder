#pragma once
#include <vector>
#include "point.h"

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
        Triangle(Point normal, Point p1, Point p2, Point p3);
        ~Triangle();

        Point P1();
        Point P2();
        Point P3();
        Point Normal();

        std::vector<Point> Points();
    };
}