#include "mainwindow.h"

#include <QApplication>
#include "include.h"
#include<QtDataVisualization/q3dbars.h>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    qRegisterMetaType<StartEnd>("StartEnd");

    MainWindow w;
    w.show();
    return a.exec();
}
