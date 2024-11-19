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
	public:
		Point();
		Point(double x,double y, double z,int id);
		~Point();
		double getZ() ;
		double getId() ;
		void setZ(double zVal);
		std::vector<double> getCoords();
		bool operator<(const Point& other);
	};
}