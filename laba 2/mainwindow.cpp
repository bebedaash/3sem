#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "form.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this); //создали экземпляр таймера
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timer())); //соединили событие с таймером
    timer->setInterval(1000);

    ui->lcdNumber_end->display("00:00:25"); //выводим начальное время в лсд
    ui->lcdNumber_all->display("00:00:00");


    ui->tableWidget->hide(); //скрываем таблицу
    ui->label_question->hide(); //скрываем вопросы
    ui->label_TimeEnd->hide(); //скрываем время до конца
    ui->label_AllTime->hide(); //скрываем общее время
    ui->label_results->hide(); //скрываем надпись результат
    ui->lcdNumber_all->hide(); //скрываем таймер
    ui->lcdNumber_end->hide(); //скрываем таймер

    ui->pushButton_next->setVisible(false);
    ui->pushButton_theend->setVisible(false);
    ui->groupBox->hide();
    ui->label->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

///Работа таймера
void MainWindow::on_timer() //то что будет вызываться по таймеру
{
    cointer--;
    ui->lcdNumber_end->display(Timerr(cointer)); //начинаем обратный отсчет
    ui->lcdNumber_all->display(Timerr(25-cointer)); //отображаем таймер всего времени

    if (cointer==0)
    {
        answerchi();
        MainWindow::uncheck(); //снимаем выделение с кнопок
        timer->stop(); //останавливаем таймер
        ui->pushButton_theend->click(); //нажимаем кнопку завершения
    }
}
///Класс обработки таймера
QString MainWindow::Timerr(int seconds)
{
    int sec,min,h; //сортируем время, чтобы работал только 1 таймер

    sec = seconds % 60;
    min = seconds / 60;
    h = min / 60;
    min = min % 60;

    return QString("%1:%2:%3").arg(QString::number(h), 2, '0').arg(QString::number(min), 2, '0').arg(QString::number(sec), 2, '0');
}
///Обработка нажатия ответа 1
void MainWindow::on_radioButton_answer0_clicked() //когда кнопка 1 ответа нажата
{
}
///Обработка нажатия ответа 2
void MainWindow::on_radioButton_answer1_clicked() //когда кнопка 1 ответа нажата
{
}
///Слот, обнуляющий нажатие кнопок с ответами
void MainWindow::uncheck()
{
    ui->radioButton_answer0->setAutoExclusive(false); //снимаем нажатость с кнопки 0
    ui->radioButton_answer1->setAutoExclusive(false); //снимаем нажатость с кнопки 1
    ui->radioButton_answer0->setChecked(false);
    ui->radioButton_answer1->setChecked(false);
    ui->radioButton_answer1->setAutoExclusive(true);
    ui->radioButton_answer0->setAutoExclusive(true);
}
///Когда нажата кнопка начать
void MainWindow::on_pushButton_clicked()
{
    ui->label_question->setVisible(true);
    ui->label_TimeEnd->setVisible(true);
    ui->label_AllTime->setVisible(true);
    ui->lcdNumber_all->setVisible(true);
    ui->lcdNumber_end->setVisible(true);
    ui->radioButton_answer1->setVisible(true);
    ui->radioButton_answer0->setVisible(true);
    ui->groupBox->setVisible(true);
    ui->pushButton_theend->setVisible(true);
    ui->pushButton->setEnabled(false);
    ui->pushButton_next->setVisible(true);

    timer->start();
}
/// Обработка нажатия кнопки "Далее"
void MainWindow::on_pushButton_next_clicked()
{
    answerchi();

    switch (q) //меняем значения ответов
    {
    case 0:
        ui->radioButton_answer0->setText("Арктика");
        ui->radioButton_answer1->setText("Австралия"); //да
        break;
    case 1:
        ui->radioButton_answer0->setText("жа"); //да
        ui->radioButton_answer1->setText("нна");
        break;
    case 2:
        ui->radioButton_answer0->setText("Москва"); //да
        ui->radioButton_answer1->setText("Тольятти");
        break;
    case 3:
        ui->radioButton_answer0->setText("Владимир Путин"); //да
        ui->radioButton_answer1->setText("Станислав Кудж");
        break;
    case 4:
        ui->radioButton_answer0->setText("В США"); //да
        ui->radioButton_answer1->setText("В Беларуси");
        break;
    case 5:
        ui->radioButton_answer0->setText("Южная Америка"); //да
        ui->radioButton_answer1->setText("Северная Америка");
        break;
    }
}
void MainWindow::answerchi()
{
    if (q==0)
    { //1.Австралия
        ui->label_question->setText(question[q]);
        if (ui->radioButton_answer0->isChecked())
        {
            user_answer[q] = 0;
        }
        else if (ui->radioButton_answer1->isChecked())
        {
            user_answer[q] = 1;
        }
        uncheck();
        q++;

    }
    if (q==1)
    {
        ui->label_question->setText(question[q]);
        if (ui->radioButton_answer0->isChecked())
        {
            user_answer[q-1] = 0;
        }
        else if (ui->radioButton_answer1->isChecked())
        {
            user_answer[q-1] = 1;
        }
        uncheck();

    }
    else if (q==2)
    {
        ui->label_question->setText(question[q]);
        if (ui->radioButton_answer0->isChecked())
        {
            user_answer[q-1] = 0;
        }
        else if (ui->radioButton_answer1->isChecked())
        {
            user_answer[q-1] = 1;
        }
        uncheck();
    }
    else if (q==3)
    {
        ui->label_question->setText(question[q]);
        if (ui->radioButton_answer0->isChecked())
        {
            user_answer[q-1] = 0;
        }
        else if (ui->radioButton_answer1->isChecked())
        {
            user_answer[q-1] = 1;
        }
        uncheck();
    }
    else if (q==4)
    {
        ui->label_question->setText(question[q]);
        ui->pushButton_next->setEnabled(false);
        if (ui->radioButton_answer0->isChecked())
        {
            user_answer[q-1] = 0;
        }
        else if (ui->radioButton_answer1->isChecked())
        {
            user_answer[q-1] = 1;
        }
        uncheck();
        ui->pushButton_theend->setVisible(true);
    }
    q++;
    return;
}
/// Обработка нажатия кнопки "Закончить"
void MainWindow::on_pushButton_theend_clicked()
{
    ui->label->setText(QString::number(q));

        if(ui->radioButton_answer1->isChecked() and q==5) //если выбран ответ 1
        {
            user_answer[4] = !answer[q]; //записываем в массив с ответами пользователя значение ответа
            uncheck();
        }
        else if (ui->radioButton_answer0->isChecked() and q==5)
        {
            user_answer[4] = answer[q];
            uncheck();
        }
        if (q!=5)
        {
        answerchi();
        }
        timer->stop(); //при нажатии на завершить таймер останавливается
        ui->groupBox->hide();
        ui->radioButton_answer1->setEnabled(false);
        ui->pushButton_next->hide();
        ui->pushButton->hide();
        ui->radioButton_answer0->hide();
        ui->radioButton_answer1->hide();
        ui->label_name->hide();
        ui->label_question->hide();
        ui->tableWidget->setVisible(true);
        ui->tableWidget->setEnabled(true);
        ui->label_results->setVisible(true);
        ui->tableWidget->setRowCount(5);
        ui->tableWidget->setColumnCount(2);


        QTableWidgetItem * twi; //записываем значения в первый столбец таблицы
        twi = new QTableWidgetItem;
        twi->setText("1 Вопрос");
        twi->setFlags(twi->flags()&0xfffffffd);
        ui->tableWidget->removeCellWidget(0,0);
        ui->tableWidget->setItem(0,0,twi);

        twi = new QTableWidgetItem;
        twi->setText("2 Вопрос");
        twi->setFlags(twi->flags()&0xfffffffd);
        ui->tableWidget->removeCellWidget(1,0);
        ui->tableWidget->setItem(1,0,twi);

        twi = new QTableWidgetItem;
        twi->setText("3 Вопрос");
        twi->setFlags(twi->flags()&0xfffffffd);
        ui->tableWidget->removeCellWidget(2,0);
        ui->tableWidget->setItem(2,0,twi);

        twi = new QTableWidgetItem;
        twi->setText("4 Вопрос");
        twi->setFlags(twi->flags()&0xfffffffd);
        ui->tableWidget->removeCellWidget(3,0);
        ui->tableWidget->setItem(3,0,twi);

        twi = new QTableWidgetItem;
        twi->setText("5 Вопрос");
        twi->setFlags(twi->flags()&0xfffffffd);
        ui->tableWidget->removeCellWidget(4,0);
        ui->tableWidget->setItem(4,0,twi);

        QString str;
        for (int i=0; i<5; i++)
        {
            if (user_answer[i]==2)
            {
                str = "Не отвечено";
            }
            else if (answer[q]==user_answer[i])
            {
                str = "Верно";
            }
            else
            {
                str = "Неверно";
            }

            twi = new QTableWidgetItem;
            twi->setText(str);

            QColor colour=Qt::yellow;
            if (str == "Верно")
            {
                colour = Qt::green;
            }
            else if (str == "Неверно")
            {
                colour = Qt::red;
            }

            twi->setFlags(twi->flags()&0xfffffffd); //записываем значения во второй столбец таблицы
            twi->setBackgroundColor(colour);
            ui->tableWidget->removeCellWidget(0,2);
            ui->tableWidget->setItem(i,1,twi);
        }
}

