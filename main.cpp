#include <QtGui/QApplication>
#include <QtCore>
#include <QtGui>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    srand(time(NULL)); //Attention multiple SRAND
    QApplication a(argc, argv);
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);
    MainWindow w;
    w.show();

    return a.exec();
}
