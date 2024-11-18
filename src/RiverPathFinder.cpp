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
    //openglWidgetOutput = new OpenGlWidgetLines(this);
    openglWidgetOutput1 = new OpenGlWidget(this);
    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput1);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0);
    layout->addWidget(findPath, 0, 1);
    layout->addWidget(openglWidgetInput, 1, 0);
    layout->addWidget(openglWidgetOutput1, 1, 1);


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
    PathFormulater pathformulater(graph, 1);
    auto riverPath = pathformulater.findPathToPoint(207);

    auto &points = reader.getPoints();
    for (int j = 0; j < riverPath.size(); j++)
    {
        int i = riverPath[j] - 1;
        qDebug() << "Point ID:" << points[i].mID
            << "X:" << points[i].getX()
            << "Y:" << points[i].getY()
            << "Z (Modified):" << points[i].getZ();
    }
    OpenGlWidget::Data newData = convertDataToGraphicsObject(reader);
    openglWidgetOutput1->setData(newData);
}


OpenGlWidget::Data RiverPathFinder::convertDataToGraphicsObject(STLFileReader& reader)
{
    std::vector<Geometry::Triangle>triangles = reader.getTriangles();

    OpenGlWidget::Data data;
    auto &triangles = reader.getTriangles();
    for (auto &triangle : triangles)
    {
        for (auto &point : triangle.Points())
        {
            data.vertices.push_back(point.getX());
            data.vertices.push_back(point.getY());
            data.vertices.push_back(point.getZ());
        }
        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(normal.getX());
            data.normals.push_back(normal.getY());
            data.normals.push_back(normal.getZ());
        }
    }
    return data;
}

OpenGlWidgetLines::Data RiverPathFinder::convertDataToGraphicsObject1(STLFileReader& reader)
{
    OpenGlWidgetLines::Data data;
    auto &triangles = reader.getTriangles();
    for (auto &triangle : triangles)
    {
        for (auto &point : triangle.Points())
        {
            data.vertices.push_back(point.getX());
            data.vertices.push_back(point.getY());
            data.vertices.push_back(point.getZ());
        }
        Point normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(normal.getX());
            data.normals.push_back(normal.getY());
            data.normals.push_back(normal.getZ());
        }
    }
    return data;
}
