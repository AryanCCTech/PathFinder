#pragma once
#include <vector>

namespace Geometry
{
	class Point
	{
	private:
	public:
		double mX;
		double mY;
		double mZ;
		int mID;
		Point();
		Point(double x,double y, double z,int id);
		~Point();
		float getY();
		std::vector<double> coord();
		bool operator<(const Point& other);
	};
}