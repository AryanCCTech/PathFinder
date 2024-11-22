#pragma once
#include "Point.h"
#include <vector>
#include <unordered_set>
#include "Graph.h"
#include "STLFileReader.h"

class PathFormulater 
{
    std::vector<int> path;         
    std::unordered_set<int> visited;
    Graph& graph;
    int startPointId;
public:
    PathFormulater(Graph& graph,int startPointId);
    ~PathFormulater();
    std::vector<int> findPath();
    std::vector<int> getPath();
    std::vector<int> findPathToPoint(int targetPointId,STLFileReader& reader);
    void setDescendingZValues(std::vector<int> path,STLFileReader& reader);
    void updateTriangles(int id,STLFileReader& reader,double newZ);
    int getNextNodeWithLeastZ(int currentNodeId);
};
