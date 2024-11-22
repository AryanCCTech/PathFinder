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

            Geometry::Point neighbor = graph.getVertex(neighborId);
            if (neighbor.getZ() < minZ) 
            {
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
    qDebug() << "Current Node:" << currentNodeId << ", Z Value : " << graph.getVertex(currentNodeId).getZ() << ", ID Value : " << graph.getVertex(currentNodeId).getId();
    return nextNodeId;
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

    double startZ = graph.getVertex(path[0]).getZ();
    double endZ = graph.getVertex(path[path.size() - 1]).getZ();

    if (startZ < endZ) {
        std::swap(startZ, endZ);
    }
    double step = (startZ - endZ) / (path.size() - 1);

    for (size_t i = 0; i < path.size(); ++i)
    {
        double newZ = startZ - (step * i);
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
