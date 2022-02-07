#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket> //библиотека работы с UDP
#include <QNetworkInterface>
#include <QTime>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_create_clicked();
    void read_data(void); //слот для обработки поступивших данных
    void on_pushButton_send_mess_clicked();
    void oshibka();
    void spamm();
    void stop_spam();

    void on_pushButton_spam_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udpsocket = new QUdpSocket; //сокет
    QHostAddress * adress = new QHostAddress; //для адреса
    bool fl = false; //флаг
    int port, our_port; //порты
    QString adres;
    QTimer * timer; //указ на класс таймер
    int cointer; //счетчик
};

#endif // MAINWINDOW_H
