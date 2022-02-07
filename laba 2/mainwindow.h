#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMainWindow>
#include <qmainwindow.h>
#include <QTimer>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

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
    void on_timer(void);
    QString Timerr(int seconds);

    void on_pushButton_clicked();
    void on_radioButton_answer1_clicked();
    void on_pushButton_next_clicked();

    void on_pushButton_theend_clicked();

    void on_radioButton_answer0_clicked();
private:
    Ui::MainWindow *ui;
    MainWindow *nWin; //создание нового окна

    QTimer * timer; //указ на класс таймер
    int cointer = 25; //счетчик
    void uncheck();
    void answerchi();
    int q = 0;
//            int k=0;
//   const QString question[5]={"1.Что было открыто раньше?", "2.Столица России?", "3.Кто президент России?","4.Где географических поясов больше?", "5.Что ближе к экватору?"};
//   const QString answers[5] = {"Австралия", "Москва", "Владимир Путин", "В США", "Южная Америка"};
//   bool answerr[5] = {true,false,false,false,false};
//   int user_answer[5] = {3, 3, 3, 3,3};
//   QString user_answers[5] = {"Не отвечено", "Не отвечено", "Не отвечено", "Не отвечено", "Не отвечено"};
//   bool answer[5]={true,false,false,false,false};
            const QString question[5]={"1.Что было открыто раньше?", "2.Столица России?", "3.Кто президент России?","4.Где географических поясов больше?", "5.Что ближе к экватору?"};
//            const QString question[4]={"2.Столица России?", "3.Кто президент России?","4.Где географических поясов больше?", "5.Что ближе к экватору?"};
            int answer[5]={1,0,0,0,0};
            int user_answer[5]={2,2,2,2,2};
//            bool answer[5]={true,false,false,false,false};
//            bool user_answer[5]={NULL,NULL,NULL,NULL,NULL};
};


#endif // MAINWINDOW_H
