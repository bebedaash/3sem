#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    thrd1 = new my_thread();
    thrd2 = new my_thread();
    srand(200);

    llt = new ll_thread();
    llt->start();

    connect(llt,SIGNAL(status_upd()),this,SLOT(potok_info()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_run_clicked()
{
    thrd1->schet=rand()%10000; //записываем в счетчик поока случайное число в диапазоне от 0 до 9999
    thrd1->start(); //запускаем поток на выполнение (будет работать переопределенный метод run)

    thrd2->schet=rand()%10000;
    thrd2->start();

}

void MainWindow::on_pushButton_start_clicked()
{
    ui->textEdit->append("1: "+((thrd1->schet>0) ? QString::number(thrd1->schet)+ "msec":"worked") + "-2: "+((thrd2->schet>0) ? QString::number(thrd2->schet)+ "msec":"worked"));
}

void MainWindow::on_pushButton_llwork_clicked()
{
    llt->com=1;
    llt->mtx->unlock();
    ui->pushButton_llkill->setEnabled(false);
    ui->pushButton_llwork->setEnabled(false);
}

void MainWindow::on_pushButton_llkill_clicked()
{
    llt->com=2;
    llt->mtx->unlock();
    ui->pushButton_llkill->setEnabled(false);
    ui->pushButton_llwork->setEnabled(false);
}

void MainWindow::potok_info()
{
    if (llt->com==3)
    {
        llt->com=0;
        ui->textEdit->append("Stopped "+ QString::number(llt->schet));
        ui->pushButton_llkill->setEnabled(true);
        ui->pushButton_llwork->setEnabled(true);
    }
    if (llt->com==4)
    {
        llt->com=0;
        ui->textEdit->append("Ended "+ QString::number(llt->schet));
        ui->pushButton_llkill->setVisible(true);
        ui->pushButton_llwork->setVisible(true);
    }
}
