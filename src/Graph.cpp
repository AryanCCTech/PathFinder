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

void Graph::addVertex(int id, const Geometry::Point& point)
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
    for (const auto& pair : adjList) {
        qDebug() << "Vertex " << pair.first << " connects to: ";
        for (const int neighbor : pair.second) {
            qDebug() << neighbor << " ";
        }
    }
}

const std::unordered_map<int, Geometry::Point> Graph::getVertices() const
{
    return points;
}

const std::unordered_map<int, std::set<int>> Graph::getEdges() const
{
    return adjList;
}

const std::set<int> Graph::getNeighbors(int id) const
{
    return adjList.at(id);
}

const Geometry::Point Graph::getVertex(int id) const
{
    return points.at(id);
}
