#include <vector>
#include "Triangle.h"

using namespace Geometry;
Geometry::Triangle::Triangle()
{
}
Triangle::Triangle(Point normal, Point p1, Point p2, Point p3)
    : p1(p1), p2(p2), p3(p3), normal(normal)
{
}

Triangle::~Triangle()
{

}

Point& Triangle::P1()
{
    return p1;
}

Point& Triangle::P2()
{
    return p2;
}

Point& Triangle::P3()
{
    return p3;
}

Point& Triangle::Normal()
{
    return normal;
}

std::vector<Point> Triangle::getPoints()
{
    std::vector<Point> points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    return points;
}

void Geometry::Triangle::setP1(Geometry::Point& P1)
{
    p1 = P1;
}

void Geometry::Triangle::setP2(Geometry::Point& P2)
{
    p2 = P2;
}

void Geometry::Triangle::setP3(Geometry::Point& P3)
{
    p3 = P3;
}