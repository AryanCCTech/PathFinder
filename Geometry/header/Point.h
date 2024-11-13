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
	public:
		Point(double x,double y, double z);
		~Point();
		std::vector<double> coord();
		bool operator<(const Point& other);
	};
}