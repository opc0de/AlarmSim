#include <QtGui/QApplication>
#include <QTimer>
#include "mainwindow.h"
#include "splashscreen.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SplashScreen scr;

    scr.show();

    qDebug()<<"Here"<<endl;
    return a.exec();
}
