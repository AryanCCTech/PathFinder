#pragma once
#include <unordered_map>
#include <set>
#include "Point.h"

class Graph 
{
private:
    std::unordered_map<int, double> points;    
    std::unordered_map<int, std::set<int>> adjList;     
public:
    Graph();
    ~Graph();

    void addVertex(int id, double zElevation);
    void addEdge(int id1, int id2);
    void printGraph() const;
    void setVertex(int id, double zElevation);

    std::unordered_map<int, double> getVertices() ;
    std::unordered_map<int, std::set<int>> getEdges() ;
    std::set<int> getNeighbors(int id) ;
    double getVertexZ(int id) ;
    int getHighestPoint();
};
