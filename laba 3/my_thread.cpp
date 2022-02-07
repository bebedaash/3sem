#include "my_thread.h"

my_thread::my_thread()
{
    schet=0;
}

void my_thread::run()
{
    int r=schet;
    schet=-1;
    msleep(r);
    schet=r;
}
//поток ждет случайное время
//возвращает значение -1 в процессе работы или время ожидания, после выполнения
