#pragma once

#include <unordered_map>
#include <set>
#include "Point.h"

class Graph {
    public:
        Graph();
        ~Graph();

        void addVertex(int id, const Geometry::Point& point);
        void addEdge(int id1, int id2);

        void printGraph() const;

        const std::unordered_map<int, Geometry::Point> getVertices() const;
        const std::unordered_map<int, std::set<int>> getEdges() const;
        const std::set<int> getNeighbors(int id) const;     
        const Geometry::Point getVertex(int id) const;      

    private:
        std::unordered_map<int, Geometry::Point> points;    
        std::unordered_map<int, std::set<int>> adjList;     
    };
