#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer> //подкл класс
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
    void on_timer(void); //слот обработки собятия таймера

    void aaa(void);
    void bbb(void);

    void on_pushButton_Start_clicked();

    void on_pushButton_Stop_clicked();

    void on_pushButton_PAUSE_clicked();


    void on_pushButton_Armagedon_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QTimer * timer; //указ на класс таймер
    int cointer; //счетчик

    QTimer * down_timer1;
    int down_cointer;

    QTimer * tim3;
    int t3;
};

#endif // MAINWINDOW_H
