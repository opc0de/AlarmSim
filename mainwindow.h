#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include "openssl/aes.h"
#include "openssl/evp.h"
#include <QTimer>
#include <QTime>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#define KEY "TORA_SECURITY_2012"
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    bool connected;
    void closeEvent(QCloseEvent * ev);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QTcpSocket * socket;
    QTimer * timp;
    Ui::MainWindow *ui;
    QSystemTrayIcon * tray;
    int interval,apelari,old_event,alarme_count;

    void LoadSettings();
    void SaveSettings();
    int  GetRandomInterval();
    QByteArray GenerateEventString();

public slots:
    void ConexionEstablished();
    void ConexionLost();
    void eroare(QAbstractSocket::SocketError e);
    void loop();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_checkBox_clicked();
    void on_horizontalSlider_sliderMoved(int position);
    void on_pushButton_5_clicked();
    void icon_clicked(QSystemTrayIcon::ActivationReason reason);
};

class myaes
{
private:
    EVP_CIPHER_CTX en;
public:
    myaes(unsigned char *key_data, int key_data_len, unsigned char *salt);
    QByteArray encrypt(QString plaintext,int len);
};

#endif // MAINWINDOW_H
