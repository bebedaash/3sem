#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this); //создали экземпляр таймера
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timer())); //соединили событие с таймером
    cointer = 0;

    down_timer1 = new QTimer(this);
    connect(down_timer1,SIGNAL(timeout()),this,SLOT(aaa())); //соединили событие с таймером
    down_cointer=60;

    tim3 = new QTimer(this);
    connect(tim3,SIGNAL(timeout()),this,SLOT(bbb())); //соединили событие с таймером

    down_timer1->setInterval(1000); //1 сек интервал счета
    tim3->setInterval(1000); //1 сек интервал счета

    ui->label->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timer() //то что будет вызываться по таймеру
{
    cointer++;
    QString str;
    int h,m,sec,s_sec; //часы,минуты,секунды,сотые секунды

    s_sec=cointer%100;
    sec=cointer/100;
    m=sec/60;
    sec=sec%60;
    h=m/60;
    m=m%60;

    if (h<10)
        str.append("0");
    str.append(QString::number(h));
    str.append(":");
    if (m<10)
        str.append("0");
        str.append(QString::number(m));
        str.append(":");
    if(sec<10)
        str.append("0");
    str.append(QString::number(sec));
    str.append(".");
    if(s_sec<10)
        str.append("0");
    str.append(QString::number(s_sec));


    ui->lcdNumber->display(str);
    //timer->stop();
    //QMessageBox::information(this,"Timer","10 sec",QMessageBox::Ok);
}
void MainWindow::on_pushButton_Start_clicked()
{
    cointer = 0;
    timer->start(10); //10 мсекунд (0.01 сек)
}

void MainWindow::on_pushButton_Stop_clicked()
{
    timer->stop();
}

void MainWindow::on_pushButton_PAUSE_clicked()
{
    if(timer->isActive())
        timer->stop(); //останавливаем 1 таймер
    else
    {
       timer->start(); //при повторном нажатии продолжем отсчет
    }
}


void MainWindow::on_pushButton_Armagedon_clicked()
{
    ui->pushButton_Armagedon->setEnabled(false); //нельзя нажать на армагедон повторно
    down_cointer = 10 ; //значение для отсчета армагедона
    down_timer1->start(); //начинаем отсчет
}

void MainWindow::aaa()
{
    down_cointer--;

    QString str;
    int h,m,sec; //часы,минуты,секунды,сотые секунды

    sec=down_cointer%60;
    m=down_cointer/60;
    sec=sec%60;
    h=m/60;
    m=m%60;

    if (h<10)
        str.append("0");
    str.append(QString::number(h));
    str.append(":");
    if (m<10)
        str.append("0");
        str.append(QString::number(m));
        str.append(":");
    if(sec<10)
        str.append("0");
    str.append(QString::number(sec));

     ui->lcdNumber_down->display(str);
     if (down_cointer==0)
     {
         down_timer1->stop(); //конец отсчет армагедона
         timer->stop(); //остановка 1 таймера
         tim3->stop(); //остановка 2 таймера
         ui->label->setVisible(true); //показывает "бум"
         ui->pushButton_Stop->click();
         ui->pushButton_Stop->setEnabled(false);     //блокируем кнопки старт, стоп, пауза, кнопка
         ui->pushButton_Start->setEnabled(false);
         ui->pushButton_PAUSE->setEnabled(false);
         ui->pushButton->setEnabled(false);
         ui->spinBox->setEnabled(false); //блокируем 1 и 2 спин бокс
         ui->spinBox_2->setEnabled(false);
     }
}

bool fl=false; //флаг

void MainWindow::bbb()
{
    t3--;

    QString str;
    int h,m,sec; //часы,минуты,секунды,сотые секунды

    sec=t3%60;
    m=t3/60;
    sec=sec%60;
    h=m/60;
    m=m%60;
    if (h<10)
        str.append("0");
    str.append(QString::number(h));
    str.append(":");
    if (m<10)
        str.append("0");
        str.append(QString::number(m));
        str.append(":");
    if(sec<10)
        str.append("0");
    str.append(QString::number(sec));

    ui->lcdNumber_2->display(str);

    if (t3==0) //когда 1 спин бокс закнчил отсчет
    {

        tim3->stop(); //остановка таймера
        //ui->pushButton_Start->click();
        t3=ui->spinBox_2->value(); //записываем значение из 2 спин бокса
        if ((fl==false) and (t3>0)) //если значение больше 0
        {
            ui->pushButton_Start->click(); //начинаем отсчет 1 таймера
            tim3->start(); //начинаем отсчет значений из 2 спин бокса
            fl=true; //меняем значение флага

        }
        else
        {
            timer->stop(); //чтобы не считало в отриательную сторону
        }
  }
}
void MainWindow::on_pushButton_clicked()
{
    fl=false; //при нажатии на кнопку меняем значение флага
    t3=ui->spinBox->value(); //задаем начальное значение из 1 спин бокса
    if(ui->spinBox->value()){
    tim3->start(); //начинаем отсчет
    }
    else {
        QMessageBox::information(this,"Ошибка","Введите количество секунд",QMessageBox::Ok);
    }

    if (t3==0) //когда отсчет закончен
    {
            tim3->stop(); //остановка таймера
            fl=true; //меняем значение флага
    }
    return;

}
