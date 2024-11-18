#include "PathFormulater.h"
#include "Graph.h"
#include "Point.h"
#include <limits>
#include <stdexcept>
#include <QDebug>
#include <stack>
#include <queue>

PathFormulater::PathFormulater(Graph& graph, int startPointId)
    : graph(graph), startPointId(startPointId) 
{

}

PathFormulater::~PathFormulater()
{
}

std::vector<int> PathFormulater::findPath() 
{
    int currentNodeId = startPointId;
    path.push_back(currentNodeId);
    visited.insert(currentNodeId);

    while (true) {
        int nextNodeId = getNextNodeWithLeastZ(currentNodeId);

        if (nextNodeId == -1) 
        {
            break;
        }

        path.push_back(nextNodeId);
        visited.insert(nextNodeId);
        currentNodeId = nextNodeId;
    }

    return path;
}

std::vector<int> PathFormulater::getPath()
{
    return path;
}

int PathFormulater::getNextNodeWithLeastZ(int currentNodeId)
{
    double minZ = graph.getVertex(currentNodeId).getZ();
    int nextNodeId = -1;

    try 
    {
        auto neighbors = graph.getNeighbors(currentNodeId);
        bool hasUnvisitedNeighbors = false;
        for (const auto& neighborId : neighbors) 
        {
            if (!visited.count(neighborId))
            {
                hasUnvisitedNeighbors = true;
                break;
            }
        }

        if (!hasUnvisitedNeighbors) 
        {
            return -1;
        }

        for (auto& neighborId : neighbors) 
        {
            if (visited.count(neighborId)) continue;

            Geometry::Point& neighbor = graph.getVertex(neighborId);
            if (neighbor.getZ() < minZ) {
                minZ = neighbor.getZ();
                nextNodeId = neighborId;
            }
        }
    }
    catch (const std::out_of_range& e) 
    {
        qDebug() << "Error: Node" << currentNodeId << "has no neighbors.";
        return -1;
    }
    qDebug() << "Current Node:" << currentNodeId << ", X Value:" << graph.getVertex(currentNodeId).getX() << ", Y Value:" << graph.getVertex(currentNodeId).getY() << ", Z Value:" << graph.getVertex(currentNodeId).getZ();
    return nextNodeId;
}

std::vector<int> PathFormulater::findPathToPoint(int targetPointId)
{
    if (visited.count(startPointId) == 0)
    {
        visited.insert(startPointId);
    }

    // Map to store parent nodes for reconstructing the path
    std::unordered_map<int, int> parentMap;
    std::queue<int> nodeQueue; // BFS queue
    nodeQueue.push(startPointId);
    parentMap[startPointId] = -1;

    while (!nodeQueue.empty())
    {
        int currentNodeId = nodeQueue.front();
        nodeQueue.pop();

        if (currentNodeId == targetPointId)
        {
            // Path found, reconstruct it
            std::vector<int> path;
            for (int id = targetPointId; id != -1; id = parentMap[id])
            {
                path.push_back(id);
            }
            std::reverse(path.begin(), path.end());

            // Modify Z values to be in descending order
            setDescendingZValues(path);
            return path;
        }

        try
        {
            auto neighbors = graph.getNeighbors(currentNodeId);
            for (const auto& neighborId : neighbors)
            {
                if (parentMap.find(neighborId) == parentMap.end()) // If not already visited
                {
                    parentMap[neighborId] = currentNodeId;
                    nodeQueue.push(neighborId);
                }
            }
        }
        catch (const std::out_of_range& e)
        {
            qDebug() << "Error: Node" << currentNodeId << "has no neighbors.";
        }
    }

    return {}; // Return an empty path if the target is unreachable
}

void PathFormulater::setDescendingZValues(std::vector<int>& path)
{
    if (path.empty())
    {
        return;
    }

    double startZ = graph.getVertex(path[0]).getZ();
    double endZ = graph.getVertex(path[path.size() - 1]).getZ();

    if (startZ < endZ) {
        std::swap(startZ, endZ);
    }
    double step = (startZ - endZ) / (path.size() - 1);

    for (size_t i = 0; i < path.size(); ++i)
    {
        double newZ = startZ - (step * i);

        graph.getVertex(path[i]).setZ(newZ);

        qDebug() << "Setting Z value for Point ID:" << path[i]
            << "to" << graph.getVertex(path[i]).getZ();;
    }
}
