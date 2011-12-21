#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QTime>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTime time;
    //time.start();

    //QSplashScreen *splash = new QSplashScreen(QPixmap("://splash"));
    //splash->show();

    MainWindow w;
    w.show();

   // while (time.elapsed() < 200)
   //     a.processEvents();

    //splash->finish(&w);
    //delete splash;

    return a.exec();
}
