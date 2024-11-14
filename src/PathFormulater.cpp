#include "PathFormulater.h"
#include "Graph.h"
#include <limits>

PathFormulater::PathFormulater(Graph& graph, Geometry::Point& startPoint)
    : graph(graph), startPoint(startPoint) {}

PathFormulater::~PathFormulater()
{
}

std::vector<int> PathFormulater::findPath() {
    int currentNodeId = startPoint.mID;
    path.push_back(currentNodeId);  
    visited.insert(currentNodeId);  

    while (true) {
        int nextNodeId = getNextNodeWithLeastY(currentNodeId);
        if (nextNodeId == -1) break; 
        path.push_back(nextNodeId);    
        visited.insert(nextNodeId);    
        currentNodeId = nextNodeId;    
    }

    return path;
}

int PathFormulater::getNextNodeWithLeastY(int currentNodeId) 
{
    double minY = std::numeric_limits<double>::max();
    int nextNodeId = -1;

    for (const auto& neighborId : graph.getNeighbors(currentNodeId)) 
    {
        if (visited.count(neighborId)) continue; 

        const Geometry::Point& neighbor = graph.getVertex(neighborId);
        if (neighbor.mY < minY) 
        {  
            minY = neighbor.mY;
            nextNodeId = neighborId;
        }
    }

    return nextNodeId; 
}
