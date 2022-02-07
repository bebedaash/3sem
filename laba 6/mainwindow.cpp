#include "mainwindow.h"
#include "ui_mainwindow.h"

///Конструктор
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    status=false;

    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    for(int nIter=0; nIter<list.count(); nIter++)  //выводим ip

    {
        if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol)
            ui->textEdit_ip->append(list[nIter].toString());

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
    ui->label_userName->hide();

    ui->lineEdit_port->hide();
    ui->lineEdit_adres->hide();
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_userName->hide();
    ui->textEdit_ip->hide();

    timer = new QTimer(this); //создали экземпляр таймера
    connect(timer,SIGNAL(timeout()),this,SLOT(spamm())); //соединили событие с таймером
    timer->setInterval(1000); //задали интервал таймеру
}

///Деструктор
MainWindow::~MainWindow()
{
    delete ui;
    clear_server();
    clear_socket();
}

///Когда выбран режим "Клиент"
void MainWindow::on_radioButton_client_clicked()
{
    //показываем необходимый для режима клиент интерфейс
    ui->label_adres->show();
    ui->lineEdit_adres->show();
    ui->label_port->show();
    ui->lineEdit_port->show();
    ui->label_ip->show();
    ui->textEdit_ip->show();
    ui->pushButton_create->hide();
    ui->pushButton_connect->show();
    ui->label_userName->show();
    ui->lineEdit_userName->show();

    if (tcpServer!=nullptr) //если меняем режим на сервер
    {
        clear_server();
        clear_socket();
        QString port = ui->lineEdit_port->text();
        ui->textEdit->append("Закрыт сервер № " + port.toUtf8());
        ui->pushButton_spam->hide();
        stop_spam();
    }
}
///Когда выбран режим "Сервер"
void MainWindow::on_radioButton_server_clicked()
{
    //показываем необходимый для режима сервер интерфейс
    ui->label_adres->hide();
    ui->lineEdit_adres->hide();
    ui->label_port->show();
    ui->lineEdit_port->show();
    ui->pushButton_connect->hide();
    ui->textEdit_ip->show();

    ui->label_ip->show();

    ui->pushButton_create->show();
    ui->lineEdit->setEnabled(false);
    ui->pushButton_send->setEnabled(false);
    if(TCPSocket_users != nullptr) //если меняем режим на клиент, то сервер закрывается
    {
        clear_socket();
        ui->textEdit->append("Клиент отключился");
        QString port = ui->lineEdit_port->text();
        ui->textEdit->append("Закрыт сервер № " + port.toUtf8());
        ui->pushButton_spam->hide();
        stop_spam();
    }
}
///Когда нажата кнопка "Создать"
void MainWindow::on_pushButton_create_clicked()
{
    if(tcpServer == nullptr) //если сервер еще не открыт
    {
        int port;
        bool fl;
        port=ui->lineEdit_port->text().toInt(&fl); //записываем значение из порта
        if(!fl) //проверка порта на число
        {
            QMessageBox::information(this,"Ошибка","В порт введите число");
        }
        else if(port <= 0 or port > 65535) //можно вводить только порт из этого диапазона
        {
            QMessageBox::information(this,"Ошибка","Неверное значение порта");
        }
        if (port>0)
        {
            clear_server();
            clear_socket();
            tcpServer=new QTcpServer(this); //создаем экземпляр сервера
            connect(tcpServer,SIGNAL(newConnection()),this,SLOT(onConnection())); //подключаем слоты

            if(tcpServer->listen(QHostAddress::Any,quint16(port)))//запуск сервера
            {
                ui->textEdit->append("Успешно был создан TCP-сервер № "+ QString::number(port));
                ui->pushButton_create->setText("Закрыть");
            }
            else //если не удалось открыть сервер
            {
                QMessageBox::information(this,"Ошибка","Этот порт уже открыт");
                tcpServer->deleteLater(); //закрываем сервер
                tcpServer = nullptr;
            }
        }
    }
    else
    {
        QString portik = ui->lineEdit_port->text();
        clear_server();
        clear_socket();
        ui->textEdit->append("Закрыт сервер №" + portik.toUtf8());
        ui->pushButton_spam->hide();
        ui->pushButton_stopSpam->hide();

    }
}
///Обработка входящих покдлючений
void MainWindow::onConnection()
{
    QTcpSocket *socket = tcpServer->nextPendingConnection(); //возврашаме ожидающее подключение в виде подключенного объекта
    if(socket != nullptr)
    {
        connect(socket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(onSocketDisconneted()));
        tcpClientList.append(socket); //добавляем сокет в лист-вектор
        ui->lineEdit->setEnabled(true);
        ui->pushButton_send->setEnabled(true);
        ui->pushButton_spam->show(); //добавила
    }
}
///Пришли данный на сервер
void MainWindow::onSocketReadyRead()
{
    QTcpSocket *senderSocket = static_cast<QTcpSocket *>(sender()); //создаем статический сокет
    if(senderSocket != nullptr) //в сокете что-то есть
    {
        QByteArray data = senderSocket->readAll(); //считываем данные из сокета и добавляем в байтовый массив
        if(data.length() > 0) //длина больше 0
        {
            QString text = ui->lineEdit->text();
            char mode = data[0]; //мод это первый элемент массива
            data = data.mid(1); //записываем данные начиная с 1
            if(tcpServer == nullptr)
            {
                switch (mode)
                {
                case 0: //если включенрежим клиента, пользователь подключился
                {QString nick = QString::fromUtf8(data);
                    ui->textEdit->append(nick + " : вошел в чат");}
                    break;
                case 1: //пользователь отключился
                {QString nick = QString::fromUtf8(data);
                    ui->textEdit->append(nick+" : покинул чат ");}
                    break;
                case 2: //сервер отправил сообщение
                {QString text = QString::fromUtf8(data);
                    ui->textEdit->append("Сервер: "+text);}
                    break;
                case 3: //пользователь отправил сообщение
                    //получаем индекс первого байта, где у нас записан 0
                    //0- разелитель между ником и строкой
                {int nickSepIndex = data.indexOf(char(0));
                    //дополнительная проверка на склейку пакетов при спаме
                    int sepIndex = data.indexOf(char(1));
                    if(sepIndex != -1)
                    {
                        QString nick = QString::fromUtf8(data.left(nickSepIndex)); //считываем данные с начала до разделителя (не включительно)
                        QString text = QString::fromUtf8(data.mid(nickSepIndex+1, sepIndex - nickSepIndex));//считываем данные после индекса и дополнительного флага

                        ui->textEdit->append(nick+ ": "+ text);
                        data = data.mid(sepIndex + 1); //записываем данные начиная с sepIndex + 1
                        //пользоватлеь отлючился
                        nick = QString::fromUtf8(data);
                        ui->textEdit->append(nick+" : покинул чат");
                    }
                    else
                    {
                        QString nick = QString::fromUtf8(data.left(nickSepIndex));//cчитываем данные с начала до разделителя (не включительно)
                        QString text = QString::fromUtf8(data.mid(nickSepIndex + 1)); //cчитываем данные после индекса и до конца
                        ui->textEdit->append(nick+": " + text);
                    }}
                    break;
                case 5:
                {
                    ui->textEdit->append("Введите уникальный ник");}
                    break;
                }
            }
            else //если выбран режим сервера
            {
                //превращаем байты в строку
                switch (mode)
                {
                case 0: //режим клиента
                {QString nick = QString::fromUtf8(data);
                    if(userNames.contains(nick))// если ник уже существует в векторе
                    {
                        QString nick_error = "Введите уникальный ник";
                        name = true;
                        userNames.append("");
                        QByteArray names = nick_error.toUtf8();
                        names.prepend(char(5)); //добавляем чар к массиву имен
                        senderSocket->write(names);
                        senderSocket->close();
                    }
                    else
                    {
                        name = false;
                        userNames.append(nick);
                        QString addressStr = senderSocket->peerAddress().toString().split(":").last();
                        ui->textEdit->append(addressStr+" вошел в чат под именем "+ nick);
                        ui->pushButton_send->show();
                        //формируем данные для отправки другим клиентам
                        QByteArray connectData = data;
                        connectData.prepend(char(0)); //добавляем чар к массиву данных
                        for(int i =0 ;i< tcpClientList.length();i++)  // отправляем данные другим клиентам
                        {
                            if(tcpClientList[i] != senderSocket)
                                tcpClientList[i]->write(connectData);
                        }
                    }}
                    break;
                case 1: //если пользователь отправляет сообщение
                {
                    int userIndex = tcpClientList.indexOf(senderSocket); //узнаем по указателю на сокет, индекс пользователя  массиве
                    QString nick = userNames[userIndex]; //берем из массива ник пользователя, который отправил это сообщение
                    QString text = QString::fromUtf8(data);
                    ui->textEdit->append(nick+ " : "+text);

                    //формируем данные для отправки другим клиентам;
                    QByteArray messageData = data;
                    messageData.prepend(char(0)); //добавляем чар к массиву данных
                    messageData.prepend(nick.toUtf8());
                    messageData.prepend(char(3));

                    for(int i =0 ;i< tcpClientList.length();i++) //отправляем данные другим клиентам
                    {
                        if(tcpClientList[i] != senderSocket)
                            tcpClientList[i]->write(messageData);
                    }
                }
                    break;
                }
            }
        }
    }
}
///Обработка подключения пользователя
void MainWindow::on_pushButton_connect_clicked()
{

    if(TCPSocket_users == nullptr)
    {
        int port;
        bool fl= false;
        port=ui->lineEdit_port->text().toInt(&fl);
        QString ip = ui->lineEdit_adres->text();
        if(serverIP.setAddress(ip) == false) //проверка адреса
        {
            QMessageBox::information(this, "Ошибка", "Введите верный Адрес"); //выводим сообщение об ошибке
            return;
        }
        if(!fl)
        {
            QMessageBox::warning(this,"Ошибка","Введите верный порт");
            return;
        }
        if(port<0 or port>65535)
        {
            QMessageBox::warning(this,"Ошибка","Введите верный порт");
            return;
        }
        QString nick = ui->lineEdit_userName->text();
        // Берём ник с формы (убираем пробелы с начала и конца строки)
        if(nick == nullptr)
        {
            QMessageBox::warning(this,"Ошибка","Введите ник");
        }
        else
        {
            clear_server();

            if (TCPSocket_users !=nullptr)
            {
                TCPSocket_users->disconnect();
                TCPSocket_users->deleteLater();
            }
            TCPSocket_users = new QTcpSocket;
            ui->lineEdit->setEnabled(true);
            ui->pushButton_send->show();
            ui->pushButton_spam->show();
            ui->lineEdit_userName->setEnabled(false);
            TCPSocket_users->connectToHost(ui->lineEdit_adres->text(),quint16(port));//Пытается установить соединение с адресом на порт

            if(TCPSocket_users->waitForConnected(5000))
            {
                if(!name)
                {
                    ui->textEdit->append("Вы подключились к серверу №  "+ QString::number(port));
                    ui->pushButton_send->setEnabled(true);
                    ui->pushButton_connect->setText("Отключиться");
                }

                connect(TCPSocket_users,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));
                connect(TCPSocket_users,SIGNAL(disconnected()),this,SLOT(onSocketDisconneted()));


                // Отправляем серверу свой ник
                QByteArray Nicks = nick.toUtf8();
                Nicks.prepend(char(0));
                TCPSocket_users->write(Nicks);
            }
            else
            {
                QMessageBox::information(this,"Ошибка","Ошибка подключения");
                TCPSocket_users->deleteLater();
                TCPSocket_users = nullptr;
            }
        }
    }
    else
    {
        clear_server();
        clear_socket();
    }
}
///Когда нажата кнопка отправить
void MainWindow::on_pushButton_send_clicked()
{
    QString text = ui->lineEdit->text();
    if(text.length() > 0 )
    {
        if (ui->radioButton_client->isChecked())
        {
            ui->textEdit->append(text);
        }
        else if (ui->radioButton_server->isChecked())
        {
            ui->textEdit->append(text);
        }
        //приводим текст в байты
        QByteArray data = text.toUtf8();
        // Если включён режим сервера, то мы должны вставить команду 2
        // Если включён режим клиента, то команду 1
        if(tcpServer == nullptr)
        {
            data.prepend(char(1));
            TCPSocket_users->write(data);
        }
        else
        {
            data.prepend(char(2));
            // если есть активные подключения
            for(int i=0; i<tcpClientList.length(); i++)
            {
                tcpClientList[i]->write(data);
            }
        }
        //очищаем поле ввода и ставим фокуас на непм
        ui->lineEdit->clear();

    }
}
///Закрытие сервера
void MainWindow::onSocketDisconneted()
{
    if(ui->radioButton_client->isChecked())
    {
        if(!name)
            ui->textEdit->append("Вы отключились");
        ui->pushButton_spam->hide();
        ui->pushButton_stopSpam->hide();
        stop_spam();
        // Удаляем единственный сокет подключения
        if(!socketClearing)
        {
            clear_socket();
        }
    }
    else
    {
        if(!socketClearing)
        {
            ui->textEdit->append("Клиент покинул чат");
            stop_spam();
            QTcpSocket *close_socket = static_cast<QTcpSocket *>(sender());//узнаем какой клиент закрыл соединение

            // Узнаём по указателю на сокет, индекс пользователя в массиве
            int userIndex =tcpClientList.indexOf(close_socket);
            QString nick = userNames[userIndex];
            QByteArray disconnectData = nick.toUtf8();
            disconnectData.prepend(char(1));
            //отправляем данные другим пользователмя
            for(int i = 0; i< tcpClientList.length();i++)
            {
                if(tcpClientList[i] != close_socket)
                    tcpClientList[i]->write(disconnectData);
            }
            tcpClientList.removeAt(userIndex);
            userNames.removeAt(userIndex);
            close_socket->deleteLater();
        }
    }
    if(tcpClientList.length()== 0) // если отключились все клиенты
    {
        ui->pushButton_send->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->pushButton_spam->hide();
    }
}


void MainWindow::clear_server()//отчистка сервера
{
    if (tcpServer != nullptr)
    {
        if (tcpServer->isListening())//если сервер слушает порт, то закрываем его
        {
            tcpServer->close();
            tcpServer->deleteLater(); //удаляем класс сервера
            tcpServer = nullptr;
        }
    }

    ui->pushButton_create->setText("Создать");
    ui->pushButton_spam->hide();
}
///Очистка сокета
void MainWindow::clear_socket()
{
    socketClearing = true; //меняем флаг
    for (int i = 0; i < tcpClientList.length(); i++) //для каждого сокета в векторе
    {
        // Если сокет открыт, то закрываем соединение
        if(tcpClientList[i] != nullptr)
        {
            if (tcpClientList[i]->isOpen())
            {
                tcpClientList[i]->close();
                tcpClientList[i]->disconnect();
            }
            // и отправляет класс на удаление
            tcpClientList[i]->deleteLater();
            tcpClientList[i] = nullptr;
        }
    }
    tcpClientList.clear(); //удаляем оставшиеся указатели

    if (TCPSocket_users != nullptr)

    {
        if (TCPSocket_users->isOpen()) //если сокет открыт, то закрываем соединение
        {
            TCPSocket_users->close();
            TCPSocket_users->disconnect();
        }
        // и отправляет класс на удаление
        TCPSocket_users->deleteLater();
        TCPSocket_users = nullptr;
    }

    socketClearing = false; //отключаем флаг
    userNames.clear();
    ui->pushButton_send->setEnabled(false);
    ui->pushButton_connect->setText("Подключиться");
    ui->lineEdit_userName->setEnabled(true);
}
///Обработчик спам
void MainWindow::spamm()
{
    cointer--; //уменьшается счетчик
    //когда счетчик нулевый
    if (cointer==0)
    {
        timer->stop(); //происходит остановка таймера
        ui->pushButton_create->show(); //доступна кнопка "Создать"
        ui->pushButton_spam->show(); //кнопка "Спам" снова доступна
    }
    QString str_time = QTime::currentTime().toString("hh:mm:ss"); //переменная считывает текущее время
    //    ui->lineEdit->setText("Добро пожаловать! " + str_time); //выводим сообщение спама
    QString text = "Добро пожаловать! " + str_time;
    if(text.length() > 0 )
    {
        ui->textEdit->append(text);
        //приводим текст в байты
        QByteArray data = text.toUtf8();
        if(tcpServer == nullptr)
        {
            data.prepend(char(1)); //если режим клиента, то командf 1
            TCPSocket_users->write(data);
        }
        else
        {
            data.prepend(char(2)); //если режим сервера, то мы должны вставить команду 2
            for(int i=0; i<tcpClientList.length(); i++) // если есть активные подключения
            {
                tcpClientList[i]->write(data);
            }
        }
       ui->lineEdit->clear();
    }

}
///КОгда нажата кнопка спам
void MainWindow::on_pushButton_spam_clicked()
{
    cointer = 1000; //значение для количества спам сообщений
    timer->start(); //начинаем отсчет
    ui->pushButton_spam->hide(); //нельзя нажать на кнопку спам
    ui->pushButton_connect->hide(); //кнопка "Подключиться" недоступна
    ui->pushButton_create->hide();
    ui->pushButton_stopSpam->show(); //кнопка "Стоп Спам" не скрыта
}
///Слот стоп спама
void MainWindow::stop_spam()
{
    cointer = 0; //задаем счетчик
    timer->stop(); //останавливаем таймер
    //    ui->pushButton_spam->show(); //показываем кнопку спам
    ui->pushButton_spam->show(); //делаем ее доступной
    if (ui->radioButton_client->isChecked())
    {
        ui->pushButton_connect->show(); //кнопка "Подключиться" доступна
    }
    else if (ui->radioButton_server->isChecked())
    {
        ui->pushButton_create->show();
    }
    ui->pushButton_stopSpam->hide(); //кнопка "Стоп Спам" скрыта
}
///Когда нажата кнопка стоп спам
void MainWindow::on_pushButton_stopSpam_clicked()
{
    stop_spam(); //обращаемся к слоту стоп спам

}
