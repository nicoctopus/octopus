#include <QtGui/QApplication>
#include <QtCore>
#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    srand(time(NULL)); //Attention multiple SRAND
    QApplication a(argc, argv);
    MainWindow w;   
    w.show();

    return a.exec();
}
