#include "RiverPathFinder.h"
#include "STLFileReader.h"
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
    openglWidgetOutput = new OpenGlWidget(this);
    //graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

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
    for (auto i : pathformulater.path)
    {
        qDebug() << i;
    }
    /*OpenGlWidget::Data data = convertDataToGraphicsObject(reader);
    openglWidgetInput->setData(data);*/
}

OpenGlWidget::Data RiverPathFinder::convertDataToGraphicsObject(STLFileReader& reader)
{
    OpenGlWidget::Data data;
    for (auto triangle : reader.triangles)
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
