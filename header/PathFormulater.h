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
    std::vector<int> path;               
    Graph& graph;                  
    Geometry::Point startPoint;          
    std::unordered_set<int> visited;     

    int getNextNodeWithLeastY(int currentNodeId);
};
