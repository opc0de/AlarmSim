#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    old_event = 0;
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    ui->eIP->setInputMask("000.000.000.000; X");
    ui->ePORT->setInputMask("00000");
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(ConexionEstablished()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(ConexionLost()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(eroare(QAbstractSocket::SocketError)));
    LoadSettings();
    interval = 60;
    apelari = 0;
    timp = new QTimer(this);
    connect(timp,SIGNAL(timeout()),this,SLOT(loop()));
    connected = false;
    qsrand(QTime::currentTime().msec());
    ui->progressBar->setFormat("");
    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/img/alarm"));
    tray->show();
    alarme_count = 0;
    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(icon_clicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::icon_clicked(QSystemTrayIcon::ActivationReason reason)
{
    if (!this->isVisible())
        this->show();
}

void MainWindow::closeEvent(QCloseEvent * ev)
{
    if (QMessageBox::question(this,"Iesire?","Esti sigur ca vrei sa iesi?",QMessageBox::Yes,QMessageBox::No) != QMessageBox::Yes )
    {
        this->hide();
        ev->ignore();
    }
}

QByteArray MainWindow::GenerateEventString()
{
    int alarme[] = {114,115,151,152,153,154,157,158,159,161,301,302,320,323,351,382,401};
    int alarme_grave[] = {110,111,112,118,120,122,123,131,132,136,137,140};
    QString placute[] = {"3333","1111","2222"};
        alarme_count++;
        int alarma = 0;
        do
        {   if (alarme_count%11 == 0)
            alarma = alarme_grave[qrand()% ARRAY_SIZE(alarme_grave)];
            else
            alarma = alarme[qrand()% ARRAY_SIZE(alarme)];
        }
        while (alarma == old_event);
        QString str = placute[qrand()%3];
        const unsigned int salt[] = {12345,54321};
        myaes aes((unsigned char*)KEY,strlen(KEY),(unsigned char*)&salt);
        QString intro = "\12\22\220011\"*ADM-CID\"0154L0#" + str + "[";
        QByteArray dump(intro.toStdString().c_str());
        intro = "01234567890123|3" + QString::number(alarma) + " 01 001]_14:24:12,09-21-2012";
        qDebug() << intro.length() << endl;
        QByteArray bts = aes.encrypt(intro,intro.length());
        qDebug() << "BYTES : " << bts.length();
        dump.insert(dump.size(),bts);
        dump.append(0x15);
        return dump;

}

int MainWindow::GetRandomInterval()
{
    return 60 + (qrand() % 600);
}

void MainWindow::loop()
{
    apelari++;
    if (apelari == interval)
    {
        apelari = 0;
        qDebug() << "Checked..." <<endl;
        if (ui->checkBox->isChecked())
        {
            interval = GetRandomInterval();
        }
        QByteArray eveniment = GenerateEventString();
        socket->write(eveniment);
        socket->flush();
    }
    int procent = 100 - (apelari * 100) / interval;
    ui->progressBar->setValue(procent);
    int timp_ramas = interval - apelari;
    QString text;
    if (timp_ramas < 60)
    {
        text = QString("Au mai ramas %1 secunde").arg(timp_ramas);
    }
    else if (timp_ramas == 0 )
    {
        text = "Trimit eveniment!";
    }
    else if (timp_ramas > 60)
    {
       int minute = timp_ramas / 60;
       int secunde = timp_ramas % 60;
       if (minute > 1)
           text = text.sprintf("Au mai ramas %d minute si %d secunde",minute,secunde);
       else
           text = QString("Au mai ramas 1 minut si %1 secunde").arg(secunde);
    }
    ui->progressBar->setFormat(text);
    if (!connected)
    {
        ui->pushButton_4->click();
    }
}

void MainWindow::LoadSettings()
{
    QSettings setari(QDir::currentPath()+"/settings.ini",QSettings::IniFormat);
    QString ip = setari.value("Server/ip","r").toString();
    QString port = setari.value("Server/port","r").toString();
    ui->eIP->setText(ip);
    ui->ePORT->setText(port);
}

void MainWindow::eroare(QAbstractSocket::SocketError e)
{
    connected = false;
    QTcpSocket * sock = static_cast<QTcpSocket*>(this->sender());
    if (sock->isOpen()) sock->close();
    QMessageBox::critical(this,"Eroare","Eroare la conexiune ;( verificati starea serverului!","Ok");
}

void MainWindow::SaveSettings()
{
    QSettings setari(QDir::currentPath()+"/settings.ini",QSettings::IniFormat);
    QString ip = ui->eIP->text();
    QString port = ui->ePORT->text();
    setari.beginGroup("Server");
    setari.setValue("ip",ip);
    setari.setValue("port",port);
    setari.sync();
}

void MainWindow::ConexionLost()
{
    connected = false;
}

void MainWindow::ConexionEstablished()
{
    connected = true;
    this->setWindowTitle("CONNECTED");
}

MainWindow::~MainWindow()
{
    tray->hide();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    SaveSettings();
    QString ip = ui->eIP->text();
    QString port = ui->ePORT->text();
    socket->connectToHost(ip,port.toInt());
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);


}

void MainWindow::on_pushButton_2_clicked()
{
    if (!connected)
    {
        QMessageBox::critical(this,"Eroare","Trebuie sa fii conectat ca sa te deconectezi.Te rog sa incerci sa te conectezi la server.\r\n\r\nDaca nu merge reporneste serverul","Bine");
    }
    else
    {
        connected = false;
        socket->disconnect();
        socket->close();
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    interval = 60 * value;
    apelari = 0;
}

void MainWindow::on_pushButton_3_clicked()
{
    if (connected)
    {
     /*   char * key = "TORA_SECURITY_2012";
        myaes aes((unsigned char*)key,strlen(key),(unsigned char*)&salt);
        QString test = "|3130 01 004][M00FA1211002FB]_14:24:12,09-21-2012";
        QByteArray enc = aes.encrypt(test,test.length());
        QByteArray dump("\12\22\220011\"*ADM-CID\"0154L0#2222[");
        dump.insert(dump.size(),enc);
        socket->write(dump);*/
        timp->setInterval(1000);
        timp->start();
    }
    else QMessageBox::information(this,"!!!","Lipsa conexiune server!","Bine");
}

void MainWindow::on_pushButton_4_clicked()
{
    if (timp->isActive())
    {
        timp->stop();
        ui->progressBar->setValue(0);
        apelari = 0;
    }
    else QMessageBox::information(this,"!!!","Nu esti in mod simulare");
}

void MainWindow::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked())
   {
    apelari = 0;
    interval = GetRandomInterval();
   }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    if (position == 1)
        ui->label_6->setText("1 minut");
    else
        ui->label_6->setText(QString("%1 minute").arg(position));
}

myaes::myaes(unsigned char * key_data,int key_data_len,unsigned char * salt)
{
    int x, nrounds = 5;
    unsigned char iv[16];
    memset(&iv[0],0,16);
    char * key = "0123456789012345";


  /*  x = EVP_BytesToKey(EVP_aes_128_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);*/
    EVP_CIPHER_CTX_init(&en);
    EVP_EncryptInit_ex(&en, EVP_aes_128_cbc(), NULL,(const unsigned char*) &key[0], iv);

}

QByteArray myaes::encrypt(QString plaintext, int len)
{
     int c_len = len + 128, f_len = 0;
     unsigned char *ciphertext = new unsigned char[c_len];
     qDebug() << plaintext;
     EVP_EncryptInit(&en,NULL,NULL,NULL);
     EVP_EncryptUpdate(&en, ciphertext, &c_len, (unsigned char*)plaintext.toStdString().data(), len);
     EVP_EncryptFinal_ex(&en, ciphertext+c_len, &f_len);
     len = c_len + f_len;
     QByteArray tmp;
     tmp.insert(0,(const char*)ciphertext,len);
     qDebug() << QString(tmp);
     return tmp;
}

void MainWindow::on_pushButton_5_clicked()
{
    if (connected)
    {
        QByteArray even = GenerateEventString();
        socket->write(even);
        socket->flush();
    }
    else QMessageBox::information(this,"Lipsa conexiune","Nu poti trimite eveniment daca nu esti conectat!","Ok");
}
