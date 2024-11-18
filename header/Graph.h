#pragma once

#include <unordered_map>
#include <set>
#include "Point.h"

class Graph 
{
private:
    std::unordered_map<int, Geometry::Point> points;    
    std::unordered_map<int, std::set<int>> adjList;     
public:
    Graph();
    ~Graph();

    //Operations on Graph
    void addVertex(int id, Geometry::Point& point);
    void addEdge(int id1, int id2);
    void printGraph() const;

    //Methods to retrieve vertices edges and their lists
    std::unordered_map<int, Geometry::Point> getVertices() ;
    std::unordered_map<int, std::set<int>> getEdges() ;
    const std::set<int> getNeighbors(int id) ;     
    Geometry::Point& getVertex(int id) ;
    const int getHighestPoint();
};
