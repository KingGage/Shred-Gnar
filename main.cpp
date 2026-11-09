#include "mainwindow.h"
#include "appmodel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppModel model;
    GameModel gameModel;
    MainWindow w(&model, &gameModel);
    w.show();
    return a.exec();
}
