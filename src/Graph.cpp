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

void Graph::addVertex(int id, double zElevation)
{
    points[id] = zElevation;
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

void Graph::setVertex(int id, double zElevation)
{
    points[id] = zElevation;
}

int Graph::getHighestPoint()
{
    if (points.empty())
    {
        throw std::runtime_error("Graph has no vertices.");
    }
    auto highestPointZ = points.begin()->second;
    auto highestPointYIndex = points.begin()->first;
    for (auto& pair : points)
    {
        auto point = pair.second;
        if (point > highestPointZ) 
        {
            highestPointZ = point;
            highestPointYIndex = pair.first;
        }
    }
    return highestPointYIndex;
}



std::unordered_map<int, double> Graph::getVertices() 
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

double Graph::getVertexZ(int id) 
{
    return points[id];
}
