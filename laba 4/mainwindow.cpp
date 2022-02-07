#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkDatagram>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
///Конструктор
{
    ui->setupUi(this);
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

     for(int nIter=0; nIter<list.count(); nIter++)

      {
          if(!list[nIter].isLoopback())
              if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
            ui->lineEdit_ip->setText(list[nIter].toString());

      }
//    ui->lineEdit_ip->setText(QNetworkInterface::allAddresses()[1].toString()); //выводим ip адрес компьютера
    timer = new QTimer(this); //создали экземпляр таймера
    connect(timer,SIGNAL(timeout()),this,SLOT(spamm())); //соединили событие с таймером
    timer->setInterval(1000); //задали интервал таймеру
    ui->pushButton->setVisible(false); //скрыли кнопку стоп спам
    ui->pushButton_spam->setVisible(false); //скрыли кнопку спам
}
///Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}
///Чтение данных
void MainWindow::read_data()
{
    char *date; //создали чаровый массив
    int size; //переменная размера
    size= int(udpsocket->pendingDatagramSize()); //записали размер сообщ
    if (size>0) //проверка на пустое сообщение
    {
        date =new char[size+1];
        udpsocket->readDatagram(date, size) ;
        QString str;
        str = QString::fromLocal8Bit(date,size);
        ui->textEdit_input->append("Сообщение друга : " + str);
        delete [] date;
    }
}
///Создаем соединение
void MainWindow::on_pushButton_create_clicked()
{
    if (ui->pushButton_spam->isVisible())
    {

    }
    else
    {
        ui->pushButton_spam->setVisible(true); //показываем кнопку спам, когда нажата кнопка создать соединение
    }

    if (fl) //проверка флага
    {
        udpsocket->close(); //закрываем сокет
    }

    port = ui->lineEdit_port->text().toInt(); //записываем введенный чужой порт в интовую переменную
    our_port= ui->lineEdit_ourPort->text().toInt(); //записываем введенный наш порт в интовую переменную
    adres=ui->lineEdit_adres->text(); //записываем ip друга в интовую переменную

    if (port<=0 or port>=65534) // проверка порта друга
    {
        QMessageBox::information(this, "Ошибка", "Вы неверно ввели порт друга"); //выводим сообщение об ошибке
        oshibka(); //обращаемся к слоту ошибки
    }
    else if (our_port<=0 or our_port>=65534)// проверка нашего порта
    {
        QMessageBox::information(this, "Ошибка", "Вы ввели неверный порт"); //выводим сообщение об ошибке
        oshibka(); //обращаемся к слоту ошибки
    }
    else if (adres==" ")// проверка ip друга
    {
        QMessageBox::information(this, "Ошибка", "Вы неверно ввели ip друга");
        oshibka(); //обращаемся к слоту ошибки
    }
    else
    {
        fl = true; //меняем флаг
        udpsocket= new QUdpSocket(this); //новый сокет
        adress = new QHostAddress; //создаем новый хост адрес
        udpsocket->bind(QHostAddress::Any, quint16(our_port));
        connect(udpsocket, &QUdpSocket::readyRead,this, &MainWindow::read_data); //соединяем сокет со слотом чтение данных
        adress->setAddress(adres); //записываем адрес
        ui->textEdit_output->append("Чат начат"); //выводим сообщение о начале чата
    }
}
///Слот обработки ошибки
void MainWindow::oshibka()
{
    if (fl)
    {
        ui->textEdit_output->append("Чат завершен");
        fl=false;
    }

}
///Кнопка "Отправить сообщение"
void MainWindow::on_pushButton_send_mess_clicked()
{
    char *date; //создали чаровый массив
    int size; //переменная размер массива
    size = ui->lineEdit_mess->text().length(); //записали длину сообщ
    QByteArray ba = ui->lineEdit_mess->text().toLocal8Bit(); //преобразовали
    date=ba.data();
    if (size>0) //проверка размера сообщения
    {
        udpsocket->writeDatagram(date, size, *adress, quint16(port));
        ui->textEdit_output->append("Мое сообщение: "  +QString::fromLocal8Bit(date)); //выводим сообщение
    }
    else
    {
        QMessageBox::information(this, "Ошибка", "Нельзя отрпавить пустое сообщение");
    }
}
///Слот спам
void MainWindow::spamm()
{
    cointer--;
    if (cointer==0)
    {
        timer->stop();
        ui->pushButton_create->setEnabled(true);
        ui->pushButton_spam->setEnabled(true);
    }
    QString str_time = QTime::currentTime().toString("hh:mm:ss"); //переменная считывает текущее время
    ui->lineEdit_mess->setText("Добро пожаловать! " + str_time); //выводим сообщение спама
    ui->pushButton_send_mess->click(); //отправка
    ui->lineEdit_mess->clear(); //очищаем строку сообщения
}
///Работа кнопки спам
void MainWindow::on_pushButton_spam_clicked()
{
    ui->pushButton->setVisible(true); //показываем стоп спам
    cointer = 1000; //значение для количества спам сообщений
    timer->start(); //начинаем отсчет
    ui->pushButton_spam->setEnabled(false); //нельзя нажать на кнопку спам
}

/////Слот остановка таймера
void MainWindow::stop_spam()
{
    cointer = 0; //задаем счетчик
    timer->stop(); //останавливаем таймер
    ui->pushButton_spam->setVisible(true); //показываем кнопку спам
    ui->pushButton_spam->setEnabled(true); //делаем ее доступной
    ui->pushButton_create->setVisible(true); //показываем кнопку создать соединение
    ui->pushButton_create->setEnabled(true); //делаем ее доступной
}
///Кнопка стоп спам
void MainWindow::on_pushButton_clicked()
{
    stop_spam(); //обращаемся к слоту стоп спам
}
