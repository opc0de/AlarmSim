#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QDialog>
#include <QTimer>
#include "mainwindow.h"
#include <QDebug>

#define SPLASH_INTERVAL 5000

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QDialog
{
    Q_OBJECT
    
public:

    MainWindow * w;
    explicit SplashScreen(QWidget *parent = 0);
    ~SplashScreen();
    
private:
    QTimer * timer;
    Ui::SplashScreen *ui;
public slots:
    void finish();
};


#endif // SPLASHSCREEN_H
