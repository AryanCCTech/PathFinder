#pragma once
#include "Point.h"
#include <vector>
#include <unordered_set>
#include "Graph.h"

class PathFormulater 
{
    std::vector<int> path;         
    std::unordered_set<int> visited;     
    Graph& graph;
public:
    PathFormulater(Graph& graph,int startPointId);
    ~PathFormulater();
    std::vector<int> findPath();
    int startPointId;
    std::vector<int> getPath();
    std::vector<int> findPathToPoint(int targetPointId);
    void setDescendingZValues(std::vector<int>& path);

    int getNextNodeWithLeastZ(int currentNodeId);
};
