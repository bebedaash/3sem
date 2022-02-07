#include "ll_thread.h"

ll_thread::ll_thread()
{

    mtx = new QMutex;
    com=0;
    mtx->lock();
}

void ll_thread::run()
{
    do
    {
        mtx->lock();
        srand(clock());
        int r;
        switch (com)
        {
        case 1: //работать
            r=rand()%8000;
            break;
        case 2: //завершение
            r=rand()%4000;
            break;
        default:
            r=0;
            break;
        }

        msleep(r);
        schet=r;
        switch (com)
        {
        case 1:
            com =3;
            break;
        case 2:
            com=4;
            break;
        default:
            com=0;
            break;

        }
        emit status_upd(); //подается сигнал обновления статуса

    }
    while (com!=4);

}
