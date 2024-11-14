#include "RiverPathFinder.h"
#include "STLFileReader.h"
#include "Graph.h"

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
        STLFileReader reader;
        reader.read(inputFilePath.toStdString(),graph);
        OpenGlWidget::Data data = convertDataToGraphicsObject(graph);
        openglWidgetInput->setData(data);
    }
}

void RiverPathFinder::onFindPathClick()
{
    STLFileReader reader;
    reader.read(inputFileName.toStdString(), graph);
    OpenGlWidget::Data data = convertDataToGraphicsObject(graph);
    openglWidgetInput->setData(data);
}

OpenGlWidget::Data RiverPathFinder::convertDataToGraphicsObject(const Graph& graph)
{
    OpenGlWidget::Data data;
    return data;
}
