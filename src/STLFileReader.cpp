#include "STLFileReader.h"
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QDebug>

#define TOLERANCE 0.0000001

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

bool STLFileReader::read(const std::string& filename, Graph& graph)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::vector<Point> trianglePoints;
    std::vector<Point> normalPoints;
    double x;
    double y;
    double z;

    while (std::getline(file, line))
    {
        if (line.find("facet normal") != std::string::npos)
        {
            std::istringstream ss(line);
            std::string temp, discard;
            ss >> discard >> discard >> x >> y >> z;

            std::string normalKey = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
            if (pointMap.find(normalKey) == pointMap.end())
            {
                Point normalPoint(x, y, z);
                points.push_back(normalPoint);
                pointMap[normalKey] = pointId;
                normalPoints.push_back(normalPoint);
                pointId++;
                qDebug() << "Created Normal " << x << " " << y << " " << normalPoint.getZ() << " " << normalPoint.getId();
            }
            else
            {
            normalPoints.push_back(points[pointMap[normalKey]-1]);
            }
        }
        else if (line.find("vertex") != std::string::npos)
        {
            std::istringstream ss(line);
            std::string temp;
            ss >> temp >> x >> y >> z;
            std::string pointKey = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);

            if (pointMap.find(pointKey) == pointMap.end())
            {
                Point vertexPoint(x, y, z);
                points.push_back(vertexPoint);
                graph.addVertex(pointId, vertexPoint);
                pointMap[pointKey] = pointId;
                trianglePoints.push_back(vertexPoint);
                qDebug() << "Created Vertex " << x << " " << y << " " << vertexPoint.getZ() << " " << vertexPoint.getId();
                pointId++;
            }
            else
            {
                trianglePoints.push_back(points[pointMap[pointKey]-1]);
            }
        }

        if (trianglePoints.size() == 3 && normalPoints.size() == 1)
        {
            Triangle t = Triangle(normalPoints[0], trianglePoints[0], trianglePoints[1], trianglePoints[2]);
            triangles.push_back(t);
            qDebug() << "Created Triangle vertices: ";
            addEdgesForTriangle(trianglePoints[0], trianglePoints[1], graph);
            addEdgesForTriangle(trianglePoints[1], trianglePoints[2], graph);
            addEdgesForTriangle(trianglePoints[2], trianglePoints[0], graph);

            trianglePoints.clear();
            normalPoints.clear();
        }
    }
    graph.printGraph();
    file.close();

    for (auto& triangle : triangles)
    {
        qDebug() << "Triangle with Normal ID: " << triangle.P1().getId()
            << ", Vertex 1 ID: " << triangle.P2().getId()
            << ", Vertex 2 ID: " << triangle.P3().getId();
    }

    return true;
}


void STLFileReader::addEdgesForTriangle(Point p1, Point p2, Graph& graph)
{
    graph.addEdge(p1.getId(), p2.getId());
    graph.addEdge(p2.getId(), p1.getId());
}

std::vector<Geometry::Point>& STLFileReader::getPoints()
{
    return points;
}

std::vector<Geometry::Triangle>& STLFileReader::getTriangles()
{
    return triangles;
}

void STLFileReader::setTriangles(std::vector<Geometry::Triangle> newtriangles)
{
    triangles = newtriangles;
}
