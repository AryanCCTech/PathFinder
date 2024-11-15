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
        bool hasUnvisitedNeighbors = false;
        for (const auto& neighborId : neighbors) {
            if (!visited.count(neighborId)) {
                hasUnvisitedNeighbors = true;
                break;
            }
        }

        if (!hasUnvisitedNeighbors) {
            return -1;
        }

        for (const auto& neighborId : neighbors) {
            if (visited.count(neighborId)) continue;

            const Geometry::Point& neighbor = graph.getVertex(neighborId);
            if (neighbor.mZ < minY) {
                minY = neighbor.mZ;
                nextNodeId = neighborId;
            }
        }
    }
    catch (const std::out_of_range& e) {
        qDebug() << "Error: Node" << currentNodeId << "has no neighbors.";
        return -1;
    }
    qDebug() << "Current Node:" << currentNodeId << ", X Value:" << graph.getVertex(currentNodeId).mX << ", Y Value:" << graph.getVertex(currentNodeId).mY << ", Z Value:" << graph.getVertex(currentNodeId).mZ;
    return nextNodeId;
}
