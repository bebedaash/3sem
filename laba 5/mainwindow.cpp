#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
///Конструктор
{
    ui->setupUi(this);

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int nIter=0; nIter<list.count(); nIter++)  //выводим ip

    {
        if(!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol)
                userName=list[nIter].toString();
        ui->lineEdit_ip->setText(list[nIter].toString());

    }
    //скрываем все кнопки до момента, пока пользователь не выберет режим
    ui->pushButton_create->hide();
    ui->pushButton_connect->hide();
    ui->pushButton_send->setEnabled(false);
    ui->pushButton_spam->hide();
    ui->pushButton_stopSpam->hide();

    ui->label_port->hide();
    ui->label_adres->hide();
    ui->label_ip->hide();

    ui->lineEdit_port->hide();
    ui->lineEdit_adres->hide();
    ui->lineEdit_ip->hide();
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_userName->hide();
    ui->label_userName->hide();

    //server
    connect(&tcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));

    //client
    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(onConnected())); //отправляем сигнал при подключении к серверу
    connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected())); //отключение и отправка сигнала об отключении
    connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(slotDataReceived())); //отправляем сигнал readyRead, когда сервер отправляет данные
    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(slotSockError(QAbstractSocket::SocketError)));//исключения сети, ошибка отправки
    timer = new QTimer(this); //создали экземпляр таймера
    connect(timer,SIGNAL(timeout()),this,SLOT(spamm())); //соединили событие с таймером
    timer->setInterval(1000); //задали интервал таймеру
}

///Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

///Обработка нажатия кнопки "Клиент"
void MainWindow::on_radioButton_client_clicked()
{
    //показываем необходимый для режима клиент интерфейс
    ui->label_adres->show();
    ui->lineEdit_adres->show();
    ui->label_port->show();
    ui->lineEdit_port->show();

    ui->pushButton_connect->show();
    ui->pushButton_connect->hide();
    ui->label_ip->show();
    ui->lineEdit_ip->show();

    ui->pushButton_create->hide();
    ui->pushButton_connect->show();
    ui->lineEdit_userName->show();
    ui->label_userName->show();
}

///Обработка кнопки "Сервер"
void MainWindow::on_radioButton_server_clicked()
{
    //показываем необходимый для режима сервер интерфейс
    ui->label_adres->hide();
    ui->lineEdit_adres->hide();
    ui->label_port->show();
    ui->lineEdit_port->show();
    ui->pushButton_connect->show();
    ui->pushButton_connect->hide();

    ui->pushButton_create->show();
    ui->lineEdit->setEnabled(false);
    ui->pushButton_send->setEnabled(false);
    ui->lineEdit->setEnabled(false);
}
///Обработка подключившихся клиентов
void MainWindow::onNewConnection()
{
    //получаем сокет для связи с клиентом
    QTcpSocket* tcpClientSocket = tcpServer.nextPendingConnection();
    //сохраняем сокет в лист
    tcpClientList.append(tcpClientSocket);
    //когда данные будут получены, будет отправлено readyRead
    connect(tcpClientSocket,SIGNAL(readyRead()),this,SLOT(slotDataReceived()));
}
///Слот получения данных
void MainWindow::slotDataReceived()
{
    if(tcpSocket.bytesAvailable()){
            QByteArray buf;
            buf.resize(tcpSocket.bytesAvailable());
            tcpSocket.read(buf.data(),buf.size());
                     // Выводим сообщение в интерфейс
            QString msg = buf.data();
            ui->textEdit->append(msg);
        }
}

///Слот записывающий сообщения в лист
void MainWindow::sendMessage(const QByteArray& msg)
{
    for(int i=0;i<tcpClientList.count();i++){
           tcpClientList.at(i)->write(msg);
 }
}
///Обработка кнопки "Создать соединение"
void MainWindow::on_pushButton_create_clicked()
{
    port = ui->lineEdit_port->text().toShort();
    // Включаем сервер
    if(tcpServer.listen(QHostAddress::Any,quint16(port)))
    {
        ui->textEdit->append("TCP-сервер успешно создан");
    }
    else
    {
        ui->textEdit->append("Ошибка создания TCP-сервера");
    }
}
///Обработка кнопки "Подключиться"
void MainWindow::on_pushButton_connect_clicked()
{
    if (status == false) {// В настоящее время соединения нет, устанавливаем соединение
                     // Получаем IP, имя пользователя, ПОРТ из интерфейса
            QString ip = ui->lineEdit_ip->text();
            if(serverIP.setAddress(ip) == false){
                QMessageBox::information(this, "ошибка", "введите правильный IP-адрес");
                return;
            }
            if(ui->lineEdit_userName->text() == ""
                    ){
                QMessageBox::information(this, "ошибка", "введите никнейм чата");
                return;
            }
            userName = ui->lineEdit_userName->text();

            if(ui->lineEdit_port->text() == ""){
                QMessageBox::information(this, "ошибка", "введите правильный порт");
                return;
            }
            port = ui->lineEdit_port->text().toShort();
                     // подключаемся к серверу
            tcpSocket.connectToHost(serverIP,port);
                     status = true; // Отметить как онлайн-статус
        }
        else{
                     // Отправляем офлайн-сообщение
                     QString msg = userName + ": Покинуть чат";
            tcpSocket.write(msg.toUtf8());
            ui->textEdit->append(msg);
                     // Закрываем соединение с сервером
            tcpSocket.disconnectFromHost();
                     status = false; // Отметить как оффлайн
        }

}
///Обработка кнопки "Отправить"
void MainWindow::on_pushButton_send_clicked()
{
    if(ui->lineEdit->text() == "")
    {
           return;
       }
       QString msg = userName + ":" + ui->lineEdit->text();
            // Отправляем сообщение на сервер
       tcpSocket.write(msg.toLocal8Bit());
            // Очистить сообщение
       ui->textEdit->append(msg);

       ui->lineEdit->clear();
}
///Слот, который вызывается при подключении к серверу
void MainWindow::onConnected(void)
{
    // Включаем кнопку отправки сообщения
    ui->pushButton_send->setEnabled(true);
    ui->lineEdit->setEnabled(true);
         // Подключиться к серверу текст кнопки изменен, чтобы покинуть сервер
         ui->pushButton_connect->setText("Покинуть сервер");
         // Отключить: ввод имени пользователя порта ip
    ui->lineEdit_ip->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);
    ui->lineEdit_userName->setEnabled(false);
         // Отправляем сообщение на сервер для входа в чат
         QString msg = userName + ": Войти в чат";
//    tcpSocket.write(msg.toLocal8Bit());
    tcpSocket.write(msg.toUtf8());
    ui->textEdit->append(msg);
}
///Слот, который вызывается при отключении от сервера
void MainWindow::onDisconnected(void)
{
    ui->pushButton_send->setEnabled(true); //блокируем кнопку отправки сообщений
    ui->lineEdit_adres->setEnabled(true); //включаем строку ввода адреса
    ui->lineEdit_port->setEnabled(true); //включаем строку ввода порта
    ui->pushButton_connect->setText("Подключиться к серверу"); //меняем текст кнопки подключиться
}

///Слот для обработки сетевых исключений
void MainWindow::slotSockError(QAbstractSocket::SocketError)
{
    QMessageBox::critical(this,"Ошибка", "Вы подключаетесь к несуществующему серверу!");
    ui->pushButton_spam->hide(); //скрываем кнопку "спам"
    ui->pushButton_stopSpam->hide(); //скрываем "стп спам"
    ui->lineEdit_port->setEnabled(true); //показываем строку ввода порта
}
///Слот очистки листа с сокетами
void MainWindow::clearSockets()
{
    int length = tcpClientList.length();
    for (int i = 0; i < length; i++)
    {
        auto socket = tcpClientList[i];
        //если сокет открыт отключаемя от него
        if (socket->isOpen())
        {
            socket->close(); //закрываем сокет
            socket->disconnect(); //отсоединяем
        }
        socket->deleteLater();//отправляем на удаление
    }
    tcpClientList.clear(); //очищаем лист с со всеми сокетами
}
///Слот спама
void MainWindow::spamm()
{
    cointer--; //уменьшается счетчик
    //когда счетчик нулевый
    if (cointer==0)
    {
        timer->stop(); //происходит остановка таймера
        ui->pushButton_create->setEnabled(true); //доступна кнопка "Создать"
        ui->pushButton_spam->setEnabled(true); //кнопка "Спам" снова доступна
    }
    QString str_time = QTime::currentTime().toString("hh:mm:ss"); //переменная считывает текущее время
    ui->lineEdit->setText("Добро пожаловать! " + str_time); //выводим сообщение спама
    ui->pushButton_send->click(); //отправка
    ui->lineEdit->clear(); //очищаем строку сообщения
}
///Обработка нажатия кнопки "Спам"
void MainWindow::on_pushButton_spam_clicked()
{
    cointer = 1000; //значение для количества спам сообщений
    timer->start(); //начинаем отсчет
    ui->pushButton_spam->setEnabled(false); //нельзя нажать на кнопку спам
    ui->pushButton_connect->setEnabled(false); //кнопка "Подключиться" недоступна
    ui->pushButton_stopSpam->show(); //кнопка "Стоп Спам" не скрыта
}
///Слот стоп Спам
void MainWindow::stop_spam()
{
    cointer = 0; //задаем счетчик
    timer->stop(); //останавливаем таймер
    ui->pushButton_spam->setVisible(true); //показываем кнопку спам
    ui->pushButton_spam->setEnabled(true); //делаем ее доступной
    ui->pushButton_connect->setEnabled(true); //кнопка "Подключиться" доступна
    ui->pushButton_stopSpam->hide(); //кнопка "Стоп Спам" скрыта
}
///Обработка нажатия кнопки "Стоп Спам"
void MainWindow::on_pushButton_stopSpam_clicked()
{
    stop_spam(); //обращаемся к слоту стоп спам
}
