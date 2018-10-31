#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

#define msleep(x) usleep(x*1000)
#define PRODUCT_SPEED 1
#define CONSUM_SPEED 1
#define INIT_NUM 3
#define TOTAL_NUM 10

using namespace std;

sem_t p_sem, c_sem, sh_sem;

int num = INIT_NUM;

void product () {
    sleep (PRODUCT_SPEED);
}

int add_to_lib () {
    ++ num;
    msleep(500);
    return num;
}

void consum () {
    sleep(CONSUM_SPEED);
}

int sub_from_lib () {
    -- num;
    msleep(500);
    return num;
}

void *productor (void *args) {
    while (1) {
//        cout << p_sem;
        sem_wait (&p_sem);
        product ();
        sem_wait (&sh_sem);
        printf ("push into! tatol_num=%d\n", add_to_lib());
        sem_post (&sh_sem);
        sem_post (&c_sem);
    }
}

void *consumer (void *args) {
    while (1) {
        sem_wait (&c_sem);
        sem_wait (&sh_sem);
        printf ("pop out! tatol_num=%d\n", sub_from_lib());
        sem_post (&sh_sem);
        sem_post (&p_sem);
        consum ();
    }
}

int main () {
    pthread_t tid1, tid2, tid3;
    sem_init(&p_sem, 0, TOTAL_NUM-INIT_NUM);
    sem_init(&c_sem, 0, INIT_NUM);
    sem_init(&sh_sem, 0, 1);
    cout << p_sem <<' ' << c_sem << ' ' << sh_sem <<endl;
    pthread_create(&tid1, NULL, productor, NULL);
    pthread_create(&tid2, NULL, consumer,NULL);
    pthread_create(&tid3, NULL, productor, NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);


    return 0;
}


https://github.com/MenghaoGuo/Hello-github.git

