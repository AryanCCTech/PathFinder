#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PathFinder.h"

class PathFinder : public QMainWindow
{
    Q_OBJECT

public:
    PathFinder(QWidget *parent = nullptr);
    ~PathFinder();

private:
    Ui::PathFinderClass ui;
};
