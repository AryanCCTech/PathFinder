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
        auto triangles = reader.getTriangles();
        for (auto tri : triangles)
        {
            for (auto pt : tri.getPoints())
            {
                qDebug() << pt.getId() << pt.getZ();
            }
        }
        OpenGlWidget::Data data = convertDataToGraphicsObject(reader);
        openglWidgetInput->setData(data);
    }
}

void RiverPathFinder::onFindPathClick()
{
    qDebug() << "Find Path Clicked ";
    PathFormulater pathformulater(graph, 1);
    STLFileReader reader1 = reader;
    auto riverPath = pathformulater.findPathToPoint(121,reader1);
    auto triangles = reader1.getTriangles();
    for (auto tri : triangles)
    {
        for (auto pt : tri.getPoints())
        {
            qDebug() << pt.getId() << pt.getZ();
        }
    }
    OpenGlWidget::Data newData = convertDataToGraphicsObject(reader1);
    openglWidgetOutput1->setData(newData);
}


OpenGlWidget::Data RiverPathFinder::convertDataToGraphicsObject(STLFileReader& reader)
{
    OpenGlWidget::Data data;
    auto triangles = reader.getTriangles();
    for (auto triangle : triangles)
    {
        auto pts  = triangle.getPoints();
        for (auto point : pts)
        {
            auto coords = point.getCoords();
            data.vertices.push_back(coords[0]);
            data.vertices.push_back(coords[1]);
            data.vertices.push_back(coords[2]);
        }
        Point normal = triangle.Normal();
        auto normalCoords = normal.getCoords();
        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(normalCoords[0]);
            data.normals.push_back(normalCoords[1]);
            data.normals.push_back(normalCoords[2]);
        }
    }
    return data;
}

//OpenGlWidgetLines::Data RiverPathFinder::convertDataToGraphicsObject1(STLFileReader& reader)
//{
//    OpenGlWidgetLines::Data data;
//    auto &triangles = reader.getTriangles();
//    for (auto &triangle : triangles)
//    {
//        for (auto point : triangle.getPoints())
//        {
//            auto coords = point.getCoords();
//            data.vertices.push_back(coords[0]);
//            data.vertices.push_back(coords[1]);
//            data.vertices.push_back(coords[2]);
//        }
//        Point normal = triangle.Normal();
//        auto normalCoords = normal.getCoords();
//
//        for (size_t i = 0; i < 3; i++)
//        {
//            data.normals.push_back(normalCoords[0]);
//            data.normals.push_back(normalCoords[1]);
//            data.normals.push_back(normalCoords[2]);
//        }
//    }
//    return data;
//}
