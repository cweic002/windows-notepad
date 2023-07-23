#include "mainwindow.h"
#include <QtGlobal>
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}
