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
    std::unordered_map<std::string, int> pointMap;
    std::unordered_map<std::string, int> normalPointMap;
    int pointId = 1;
    int normalId = 1000000;
    std::vector<Geometry::Point> normals;
public:
    std::vector<Geometry::Point> points;
    std::vector<Geometry::Triangle> triangles;
    STLFileReader();
    ~STLFileReader();
    bool operator()(double a, double b) const;
    bool read(const std::string& filename, Graph& grpah);
    void addEdgesForTriangle( Geometry::Point& p1, Geometry::Point& p2,Graph& graph);/*
    const std::vector<Geometry::Point>& getPoints() const;
    const std::vector<Geometry::Triangle>& getTriangles() const;*/
};