#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include "Triangle.h"
#include "Graph.h"
#define TOLERANCE 0.0000001

class STLFileReader
{
private:
    int pointId = 1;
    int normalId = 1000000;
    std::unordered_map<std::string, int> pointMap;
    std::unordered_map<std::string, int> normalPointMap;
    
    std::vector<Geometry::Point> points;
    std::vector<Geometry::Point> normals;
    std::vector<Geometry::Triangle> triangles;

    void addEdgesForTriangle( Geometry::Point& p1, Geometry::Point& p2,Graph& graph); 
  
    bool operator()(double a, double b) const;
public:
    STLFileReader();
    ~STLFileReader();
    bool read(const std::string& filename, Graph& grpah);
    std::vector<Geometry::Point>& getPoints();
    std::vector<Geometry::Triangle>& getTriangles();

 
};