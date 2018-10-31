#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

struct shmhead {
    unsigned int blksize; // 块大小
    unsigned int blocks; // 总块数
    unsigned int rd_index; // 读索引
    unsigned int we_index;  // 写索引
};

struct shmfifo {
    shmhead_t * p_shm; // 共享内存头部指针
    char *p_payload; // 有效负载的起始地址

    int shmid; // 共享内存 ID
    int sem_mutex; // 信号量
    int sem_full; // 共享内存是否满
    int sem_empty; // 共享内存是否为空
};

shmfifo_t *shmfifo_init(int key, int blksize, int blocks) {
	 shmfifo_t *fifo = (shmfifo_t *)malloc(sizeof(shmfifo_t));
	 assert(fifo != NULL);	/*断言fifo不为空，若为空程序将崩溃*/
	 memset(fifo, 0, sizeof(shmfifo_t));

	 /* 先已打开方式调用shmget，若打开失败即说明还未创建，然后创建共享内存 */
	 int shmid;
	 shmid = shmget(key, 0, 0);
	 int size = sizeof(shmhead_t) + blksize * blocks;
	 if (shmid == -1) {
		 fifo->shmid = shmget(key, size, IPC_CREAT | 0666);
		 if (fifo->shmid == -1)
			ERR_EXIT("shmget");

		fifo->p_shm = (shmhead_t *)shmat(fifo->shmid, NULL, 0);
		if (fifo->p_shm == (shmhead_t*)-1)
			ERR_EXIT("shmat");

		/*创建三个信号量,两个信号量的key是不能相同的*/
		fifo->sem_mutex = sem_create(key);
		fifo->sem_full = sem_create(key+1);
		fifo->sem_empty = sem_create(key+2);

		/*将信号量初始化*/
		sem_setval(fifo->sem_mutex, 1);
		sem_setval(fifo->sem_full, blocks);
		sem_setval(fifo->sem_empty, 0);
        fifo->p_payload = (char*)(fifo->p_shm + 1);

                fifo->p_shm->blksize = blksize;
                fifo->p_shm->blocks = blocks;
                fifo->p_shm->rd_index = 0;
                fifo->p_shm->wr_index = 0;
	 }
	 else {/*如果打开共享内存成功，说明共享内存已经存在了*/
		fifo->shmid = shmid;
		fifo->p_shm = (shmhead_t *)shmat(fifo->shmid, NULL, 0);
		if (fifo->p_shm == (shmhead_t*)-1)
			ERR_EXIT("shmat");
		fifo->sem_mutex = sem_open(key);
		fifo->sem_full = sem_open(key+1);
		fifo->sem_empty = sem_open(key+2);
        fifo->p_payload = (char*)(fifo->p_shm + 1);
	 }
	 return fifo;
 }