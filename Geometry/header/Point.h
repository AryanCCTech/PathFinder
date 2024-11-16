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
		int mID;
		Point();
		Point(double x,double y, double z,int id);
		~Point();
		double getX() ;
		double getY() ;
		double getZ() ;
		void setZ(double zVal);
		std::vector<double> coord();
		bool operator<(const Point& other);
	};
}