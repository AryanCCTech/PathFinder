#include "Graph.h"
#include <iostream>
#include <QDebug>

using namespace Geometry;

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::addVertex(int id, Geometry::Point& point)
{
    points[id] = point;
}

void Graph::addEdge(int id1, int id2)
{
    adjList[id1].insert(id2);
}

void Graph::printGraph() const
{
    std::cout << "Graph vertices and edges:" << std::endl;
    for (const auto& pair : adjList) 
    {
        qDebug() << "Vertex " << pair.first << " connects to: ";
        for (const int neighbor : pair.second) 
        {
            qDebug() << neighbor << " ";
        }
    }
}

int Graph::getHighestPoint()
{
    if (points.empty())
    {
        throw std::runtime_error("Graph has no vertices.");
    }
    auto highestPoint = points.begin()->second;
    for (auto& pair : points)
    {
        Geometry::Point& point = pair.second;
        if (point.getZ() > highestPoint.getZ()) 
        {
            highestPoint = point;
        }
    }
    return highestPoint.getId();
}



std::unordered_map<int, Geometry::Point> Graph::getVertices() 
{
    return points;
}

std::unordered_map<int, std::set<int>> Graph::getEdges() 
{
    return adjList;
}

std::set<int> Graph::getNeighbors(int id) 
{
    return adjList.at(id);
}

Point Graph::getVertex(int id) 
{
    return points[id];
}
