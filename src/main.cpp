#include "RiverPathFinder.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RiverPathFinder w;
    w.show();
    return a.exec();
}
