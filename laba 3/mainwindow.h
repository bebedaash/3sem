#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "my_thread.h"
#include "ll_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    my_thread *thrd1;
    my_thread *thrd2;

    ll_thread *llt;

public slots:
    void potok_info(void);

private slots:
    void on_pushButton_run_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_llwork_clicked();

    void on_pushButton_llkill_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
