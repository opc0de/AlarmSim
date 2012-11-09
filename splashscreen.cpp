#include "splashscreen.h"
#include "ui_splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
    this->setStyleSheet("background-image: url(:/img/logo);"
                         "background-position: center;"
                         "background-repeat: no-repeat;");
    this->setWindowFlags(Qt::SplashScreen);
    timer = new QTimer(this);
    timer->singleShot(SPLASH_INTERVAL,this,SLOT(finish()));
}

void SplashScreen::finish()
{
    w = new MainWindow();
    w->setWindowFlags(w->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    w->show();
    this->close();
}

SplashScreen::~SplashScreen()
{
    delete w;
    delete ui;
}


