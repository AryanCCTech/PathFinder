#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QString>

#include "Triangle.h"
#include "OpenglWidget.h"
#include "Graph.h"

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
    OpenGlWidget::Data convertDataToGraphicsObject(const Graph& graph);

private:
    Graph graph;

    QPushButton* loadFile;
    QPushButton* findPath;

    OpenGlWidget* openglWidgetInput;
    OpenGlWidget* openglWidgetOutput;


    QVector<GLfloat> vertices;

    QString tempFileName;


    // translation data
    QString inputFilePath;
    QString inputFileName;
};
