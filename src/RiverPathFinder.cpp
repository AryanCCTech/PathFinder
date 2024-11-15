#include "RiverPathFinder.h"
#include "STLFileReader.h"
#include "Graph.h"
#include "GraphicsSynchronizer.h"
#include "PathFormulater.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QFile>


RiverPathFinder::RiverPathFinder(QWidget* parent) : QMainWindow(parent)
{
    setupUi();
    connect(loadFile, &QPushButton::clicked, this, &RiverPathFinder::onLoadFileClick);
    connect(findPath, &QPushButton::clicked, this, &RiverPathFinder::onFindPathClick);
}

RiverPathFinder::~RiverPathFinder()
{
}


void RiverPathFinder::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    findPath = new QPushButton("Find Path", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetOutput = new OpenGlWidgetLines(this);
    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0);
    layout->addWidget(findPath, 0, 1);
    layout->addWidget(openglWidgetInput, 1, 0);
    layout->addWidget(openglWidgetOutput, 1, 1);


    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
}


void  RiverPathFinder::onLoadFileClick()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("files (*.stl)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        reader.read(inputFilePath.toStdString(),graph);
        OpenGlWidget::Data data = convertDataToGraphicsObject(reader);
        openglWidgetInput->setData(data);
    }
}

void RiverPathFinder::onFindPathClick()
{
    qDebug() << "Find Path Clicked ";
    Geometry::Point start = graph.getHighestPoint();
    PathFormulater pathformulater(graph,start);
    pathformulater.findPath();
    std::vector<Geometry::Point>points = reader.getPoints();
    for (int j= 0; j < pathformulater.path.size();j++)
    {
        pathformulater.path[j] -= 1;
        int i = pathformulater.path[j];

        qDebug()<<  i << "Point ID " <<  points[i].mID << "X value " <<  points[i].mX << "Y value " <<  points[i].mY << "Z value " <<  points[i].mZ;
    }
    OpenGlWidgetLines::Data data1 = convertDataToGraphicsObject1(reader);
    data1.vertices.clear();
    data1.vertices.push_back( points[pathformulater.path[0]].mX);
    data1.vertices.push_back( points[pathformulater.path[0]].mY);
    data1.vertices.push_back( points[pathformulater.path[0]].mZ);
    for (int i = 1; i < pathformulater.path.size(); i++)
    {
        data1.vertices.push_back( points[pathformulater.path[i]].mX);
        data1.vertices.push_back( points[pathformulater.path[i]].mY);
        data1.vertices.push_back(  points[pathformulater.path[i]].mZ);
        data1.vertices.push_back( points[pathformulater.path[i]].mX);
        data1.vertices.push_back( points[pathformulater.path[i]].mY);
        data1.vertices.push_back( points[pathformulater.path[i]].mZ);
    }
    data1.vertices.pop_back();
    data1.vertices.pop_back();
    data1.vertices.pop_back();
    openglWidgetOutput->setData(data1);
}

OpenGlWidget::Data RiverPathFinder::convertDataToGraphicsObject(STLFileReader& reader)
{
    std::vector<Geometry::Triangle>triangles = reader.getTriangles();

    OpenGlWidget::Data data;
    for (auto triangle :  triangles)
    {
        for (auto point : triangle.Points())
        {
            data.vertices.push_back(point.mX);
            data.vertices.push_back(point.mY);
            data.vertices.push_back(point.mZ);

            bool isRiverPoint = true;
            data.highlightFlags.push_back(isRiverPoint ? 1.0f : 0.0f);
        }
        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(normal.mX);
            data.normals.push_back(normal.mY);
            data.normals.push_back(normal.mZ);
        }
    }
    return data;
}

OpenGlWidgetLines::Data RiverPathFinder::convertDataToGraphicsObject1(STLFileReader& reader)
{
    OpenGlWidgetLines::Data data;
    std::vector<Geometry::Triangle>triangles = reader.getTriangles();

    for (auto triangle :  triangles)
    {
        for (auto point : triangle.Points())
        {
            data.vertices.push_back(point.mX);
            data.vertices.push_back(point.mY);
            data.vertices.push_back(point.mZ);
        }
        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(normal.mX);
            data.normals.push_back(normal.mY);
            data.normals.push_back(normal.mZ);
        }
    }
    return data;
}
