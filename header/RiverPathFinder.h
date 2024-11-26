#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QString>

#include "Triangle.h"
#include "OpenglWidget.h"
#include "Graph.h"
#include "GraphicsSynchronizer.h"
#include "STLFileReader.h"

using namespace Geometry;

class RiverPathFinder : public QMainWindow
{
    Q_OBJECT

public:
    RiverPathFinder(QWidget *parent = nullptr);
    ~RiverPathFinder();

private slots:
    void onLoadFileClick();
    void onFindPathClick();

private:
    void setupUi();
    OpenGlWidget::Data convertDataToGraphicsObject(STLFileReader& reader);
    OpenGlWidget::Data convertDataToGraphicsObject1(STLFileReader& reader,std::vector<int> path);

private:
    STLFileReader reader;
    Graph graph;

    QPushButton* loadFile;
    QPushButton* findPath;

    OpenGlWidget* openglWidgetInput;
    OpenGlWidget* openglWidgetOutput;

    GraphicsSynchronizer* graphicsSynchronizer;

    QVector<GLfloat> vertices;

    QString tempFileName;

    QString inputFilePath;
    QString inputFileName;
};
