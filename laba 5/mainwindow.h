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

    //server
    void sendMessage(const QByteArray&);
    void onNewConnection();
    void slotDataReceived();

    //client
    void onConnected(void);
    void onDisconnected(void); //вызывается при отключении от сервера
    void slotSockError(QAbstractSocket::SocketError); //обработка сетевых исключений
    void clearSockets(); //очистка сокетов

    void spamm(); //обработка спам
    void stop_spam(); //остановка спама

    void on_pushButton_spam_clicked();
    void on_pushButton_stopSpam_clicked();

private:
    Ui::MainWindow *ui;

    //server
    QTcpServer tcpServer; // Объект сервера
    qint16 port; // порт сервера
    // Сохраняем все сокеты, взаимодействующие с клиентом
    QList <QTcpSocket*> tcpClientList;

    //client
    bool status=false; // Отмечаем статус подключения клиента, true online, false offline
    QTcpSocket tcpSocket; // Сокет для связи с сервером
    qint16 port_client; // Порт связи с сервером
    QHostAddress serverIP; // IP-адрес связи с сервером
    QString userName; //никнейм

    QTimer * timer; //указ на класс таймер
    int cointer; //счетчик
};

#endif // MAINWINDOW_H
