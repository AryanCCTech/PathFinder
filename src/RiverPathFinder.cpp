#include "RiverPathFinder.h"
#include "Graph.h"
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
    //openglWidgetOutput = new OpenGlWidgetLines(this);
    openglWidgetOutput = new OpenGlWidget(this);
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
        auto triangles = reader.getTriangles();
        for (auto tri : triangles)
        {
            for (auto pt : tri.getPoints())
            {
                qDebug() << pt.getId() << pt.getZ();
            }
        }
        OpenGlWidget::Data data = convertDataToGraphicsObject(reader);
        openglWidgetInput->addObject(data);
    }
}

void RiverPathFinder::onFindPathClick()
{
    qDebug() << "Find Path Clicked ";
    PathFormulater pathformulater(graph, 1);
    STLFileReader reader1 = reader;
    auto riverPath = pathformulater.findPathToPoint(301, reader1);
    //auto riverPath = pathformulater.findPath();
    auto triangles = reader1.getTriangles();
    for (auto tri : triangles)
    {
        for (auto pt : tri.getPoints())
        {
            qDebug() << pt.getId() << pt.getZ();
        }
    }
    //OpenGlWidget::Data data = convertDataToGraphicsObject(reader1);
    //openglWidgetOutput->addObject(data);
    OpenGlWidget::Data data1 = convertDataToGraphicsObject(reader1);
    data1.drawStyle = OpenGlWidget::LINES;
   /* data1.vertices.clear();
    data1.normals.clear();
    auto points = reader1.getPoints();
    for (auto i : riverPath)
    {
        data1.vertices.push_back(points[i].getCoords()[0]);
        data1.vertices.push_back(points[i].getCoords()[1]);
        data1.vertices.push_back(points[i].getCoords()[2]);
        data1.vertices.push_back(points[i].getCoords()[0]);
        data1.vertices.push_back(points[i].getCoords()[1]);
        data1.vertices.push_back(points[i].getCoords()[2]);
    }
    data1.vertices.pop_front();
    data1.vertices.pop_front();
    data1.vertices.pop_front();
    data1.vertices.pop_back();
    data1.vertices.pop_back();
    data1.vertices.pop_back();*/
    data1.drawStyle = OpenGlWidget::LINES;
    openglWidgetOutput->addObject(data1);
}


OpenGlWidget::Data RiverPathFinder::convertDataToGraphicsObject(STLFileReader& reader)
{
    OpenGlWidget::Data data;
    auto triangles = reader.getTriangles();
    for (auto triangle : triangles)
    {
        auto pts = triangle.getPoints();
        for (auto point : pts)
        {
            auto coords = point.getCoords();
            data.vertices.push_back(coords[0]);
            data.vertices.push_back(coords[1]);
            data.vertices.push_back(coords[2]);
        }
        Point normal = triangle.Normal();
        auto normalCoords = normal.getCoords();
        for (int i = 0; i < 3; i++)
        {
            data.normals.push_back(normalCoords[0]);
            data.normals.push_back(normalCoords[1]);
            data.normals.push_back(normalCoords[2]);
        }
        for (int i = 0; i < 3; i++)
        {
            data.colors.push_back(0.22);
            data.colors.push_back(1.0);
            data.colors.push_back(0.07);
        }
        data.drawStyle = OpenGlWidget::TRIANGLES;
    }
    return data;
}

OpenGlWidget::Data RiverPathFinder::convertDataToGraphicsObject1(STLFileReader& reader,std::vector<int> path)
{
    OpenGlWidget::Data data;
    auto points = reader.getPoints();
    for (auto i : path)
    {
        auto coords = points[i].getCoords();
        data.vertices.push_back(coords[0]);
        data.vertices.push_back(coords[1]);
        data.vertices.push_back(coords[2]);
    }
    for (int i = 0; i < 3; i++)
    {
        data.colors.push_back(0.0);
        data.colors.push_back(0.0);
        data.colors.push_back(0.9);
    }
    data.drawStyle = OpenGlWidget::LINES;
    return data;
}
