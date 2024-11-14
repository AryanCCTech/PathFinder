#include "STLFileReader.h"
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <QDebug>

using namespace Geometry;

STLFileReader::STLFileReader()
{
}

STLFileReader::~STLFileReader()
{
}

bool STLFileReader::operator()(double a, double b) const
{
    return fabs(a - b) > TOLERANCE ? a < b : false;
}

bool STLFileReader::read(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::vector<Point> trianglePoints;
    std::vector<Point> normalPoints;
    double x, y, z;

    while (std::getline(file, line)) {
        if (line.find("facet normal") != std::string::npos) {
            std::istringstream ss(line);
            std::string temp, discard;
            ss >> discard >> discard >> x >> y >> z; 

            std::string normalKey = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
            if (normalPointMap.find(normalKey) == normalPointMap.end()) {
                Point normalPoint(x, y, z, normalId);
                normals.push_back(normalPoint);
                graph.addVertex(normalId, normalPoint);
                normalPointMap[normalKey] = normalId++;
                qDebug() << "Created Normal " << normalPoint.mX << " " << normalPoint.mY << " " << normalPoint.mZ << " " << normalPoint.mID;
            }
            normalPoints.push_back(normals.back());
        }
        else if (line.find("vertex") != std::string::npos) {
            // Parse the vertex coordinates
            std::istringstream ss(line);
            std::string temp;
            ss >> temp >> x >> y >> z;

            std::string pointKey = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
            if (pointMap.find(pointKey) == pointMap.end()) {
                Point vertexPoint(x, y, z, pointId);
                points.push_back(vertexPoint);
                graph.addVertex(pointId, vertexPoint);
                pointMap[pointKey] = pointId;
                trianglePoints.push_back(vertexPoint);
                qDebug() << "Created Vertex " << vertexPoint.mX << " " << vertexPoint.mY << " " << vertexPoint.mZ << " " << vertexPoint.mID;
                pointId++;
            }
            else {
                trianglePoints.push_back(points[pointMap[pointKey] - 1]);  // Using index from pointMap
            }
        }
        if (trianglePoints.size() == 3) {
            triangles.emplace_back(normalPoints[0], trianglePoints[0], trianglePoints[1], trianglePoints[2]);
            qDebug() << "Triangle vertices:";
            qDebug() << trianglePoints[0].mID << trianglePoints[1].mID << trianglePoints[2].mID;

            addEdgesForTriangle(trianglePoints[0], trianglePoints[1], graph);
            addEdgesForTriangle(trianglePoints[1], trianglePoints[2], graph);
            addEdgesForTriangle(trianglePoints[2], trianglePoints[0], graph);

            trianglePoints.clear();
            normalPoints.clear();
        }
    }

    graph.printGraph();
    file.close();
    for (auto i : triangles)
    {
        qDebug() << i.P1().mX << " " << i.P1().mY << " " << i.P1().mZ;
        qDebug() << i.P2().mX << " " << i.P2().mY << " " << i.P2().mZ;
        qDebug() << i.P3().mX << " " << i.P3().mY << " " << i.P3().mZ;
    }
    return true;
}

void STLFileReader::addEdgesForTriangle( Geometry::Point& p1, Geometry::Point& p2, Graph& graph) {
    if (p1.getY() > p2.getY()) {
        graph.addEdge(p1.mID, p2.mID);  // Edge from higher Y to lower Y
    }
    else {
        graph.addEdge(p2.mID, p1.mID);  // Edge from higher Y to lower Y
    }
}
