#ifndef LL_THREAD_H
#define LL_THREAD_H
#include <QThread>
#include<QMutex>

class ll_thread : public QThread
{
    Q_OBJECT
public:
    ll_thread();

    void run();

    QMutex * mtx; //мьютекс

    int schet; //счетчи
    int com; //код команды

signals:
    void status_upd();
};

#endif // LL_THREAD_H
