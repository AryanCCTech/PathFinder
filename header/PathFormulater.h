#pragma once
#include "Point.h"
#include <vector>
#include <unordered_set>
#include "Graph.h"

class PathFormulater {
public:
    PathFormulater(Graph& graph,Geometry::Point& startPoint);
    ~PathFormulater();
    std::vector<int> findPath();
private:
    Graph& graph;                  
    Geometry::Point startPoint;          
    std::vector<int> path;               
    std::unordered_set<int> visited;     

    int getNextNodeWithLeastY(int currentNodeId);
};
