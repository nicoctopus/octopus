#include <QtGui/QApplication>
#include <QtCore>
#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   MainWindow w;
   w.show();


    //Forme *forme = new Forme();
    //forme->resize(100, 100);
    //forme->show();

	
    return a.exec();
}
