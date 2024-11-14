#include "PathFormulater.h"
#include "Graph.h"
#include <limits>
#include <stdexcept>
#include <QDebug>

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

        if (nextNodeId == -1) {
            // No unvisited neighbors left; end the path here
            break;
        }

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

    try {
        auto neighbors = graph.getNeighbors(currentNodeId);
        for (const auto& neighborId : neighbors) {
            if (visited.count(neighborId)) continue;

            const Geometry::Point& neighbor = graph.getVertex(neighborId);
            if (neighbor.mY < minY) {
                minY = neighbor.mY;
                nextNodeId = neighborId;
            }
        }
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: Node" << currentNodeId << "has no neighbors.";
        return -1;  // Explicitly return -1 if neighbors cannot be retrieved
    }

    return nextNodeId;
}
