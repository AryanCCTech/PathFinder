#pragma once
#include "PathFormulater.h"
#include "Graph.h"
#include "Point.h"
#include <limits>
#include <stdexcept>
#include <QDebug>
#include <stack>
#include <queue>
#include "STLFileReader.h"

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

    while (true) 
    {
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
    try
    {
        auto neighbors = graph.getNeighbors(currentNodeId);
        if (neighbors.empty())
        {
            throw std::runtime_error("No neighbors found for the current node.");
        }
        int nextNodeId = -1;
        double minZ = graph.getVertexZ(currentNodeId);
        for (int neighbor : neighbors)
        {
            if (visited.count(neighbor))
            {
                continue;
            }
            double z = graph.getVertexZ(neighbor);
            if (z < minZ)
            {
                minZ = z;
                nextNodeId = neighbor;
            }
        }
        if (nextNodeId == -1)
        {
            throw std::runtime_error("No unvisited neighbors with lower Z elevation.");
        }
        return nextNodeId;
    }
    catch (const std::out_of_range& e)
    {
        qDebug() << "Error: Invalid node ID - " << currentNodeId << ". Node not found in the graph.";
        return -1;
    }
    catch (const std::exception& e)
    {
        qDebug() << "Error: " << e.what();
        return -1;
    }
}


std::vector<int> PathFormulater::findPathToPoint(int targetPointId,STLFileReader& inputReader)
{
    if (visited.count(startPointId) == 0)
    {
        visited.insert(startPointId);
    }
    std::unordered_map<int, int> parentMap;
    std::queue<int> nodeQueue;
    nodeQueue.push(startPointId);
    parentMap[startPointId] = -1;

    while (!nodeQueue.empty())
    {
        int currentNodeId = nodeQueue.front();
        nodeQueue.pop();

        if (currentNodeId == targetPointId)
        {
            std::vector<int> path;
            for (int id = targetPointId; id != -1; id = parentMap[id])
            {
                path.push_back(id);
            }
            std::reverse(path.begin(), path.end());
            setDescendingZValues(path,inputReader);
            return path;
        }

        try
        {
            auto neighbors = graph.getNeighbors(currentNodeId);
            for (const auto& neighborId : neighbors)
            {
                if (parentMap.find(neighborId) == parentMap.end())
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

    return {}; // returning empty list if no path
}

void PathFormulater::setDescendingZValues(std::vector<int> path,STLFileReader& inputReader)
{
    if (path.empty())
    {
        return;
    }

    double startZ = graph.getVertexZ(path[0]);
    double endZ = graph.getVertexZ(path[path.size() - 1]);

    if (startZ < endZ) {
        std::swap(startZ, endZ);
    }
    double step = (startZ - endZ) / (path.size() - 1);

    for (size_t i = 0; i < path.size(); ++i)
    {
        double newZ = startZ - (step * i);
        graph.setVertex(path[i],newZ);
        updateTriangles(path[i], inputReader,newZ);
 
    }
}

void PathFormulater::updateTriangles(int id, STLFileReader& inputReader, double newZ)
{
    std::vector<Geometry::Triangle>& triangles = inputReader.getTriangles();

    for (Geometry::Triangle& triangle : triangles)
    {
        int p1Id = triangle.P1().getId();
        int p2Id = triangle.P2().getId();
        int p3Id = triangle.P3().getId();

        if (p1Id == id)
        {
            triangle.setP1ZVal(newZ);
            qDebug() << "Updated P1 Z to " << newZ;
        }

        if (p2Id == id)
        {
            triangle.setP2ZVal(newZ);
            qDebug() << "Updated P2 Z to " << newZ;
        }

        if (p3Id == id)
        {
            triangle.setP3ZVal(newZ);
            qDebug() << "Updated P3 Z to " << newZ;
        }
    }
}
