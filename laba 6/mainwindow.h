#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QNetworkInterface>
#include <QDialog>
#include <QMessageBox>
#include <QTimer>
#include <QTime>

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
    void on_radioButton_client_clicked();
    void on_radioButton_server_clicked();
    void on_pushButton_create_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_send_clicked();
    void onConnection();
    void clear_socket(); //очистка сокетов
    void clear_server();
    void onSocketDisconneted();
    void onSocketReadyRead();
    void spamm();
    void on_pushButton_spam_clicked();

    void on_pushButton_stopSpam_clicked();
    void stop_spam();

private:
    Ui::MainWindow *ui;

    //server
    QTcpServer *tcpServer=nullptr;
    QTcpSocket *TCPSocket_users = nullptr;

    qint16 port; // порт сервера
    // Сохраняем все сокеты, взаимодействующие с клиентом
    QVector <QTcpSocket*> tcpClientList;

    //client
    bool status; // Отмечаем статус подключения клиента, true online, false offline
    QTcpSocket tcpSocket; // Сокет для связи с сервером
    qint16 port_client; // Порт связи с сервером
    QHostAddress serverIP; // IP-адрес связи с сервером
    bool socketClearing=false;
    QTimer *spamTimer = nullptr;

    QVector<QString> userNames;
    bool name = false;


    QTimer * timer=nullptr; //указ на класс таймер
    int cointer; //счетчик
};

#endif // MAINWINDOW_H
