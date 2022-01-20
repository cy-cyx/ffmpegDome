//
// Created by JOYY on 2022/1/20.
// 阻塞队列
//

#include <pthread.h>
#include <ILog.h>

#ifndef FFMPEGDOME_IBLOCKQUEUE_H
#define FFMPEGDOME_IBLOCKQUEUE_H

struct Node {
    void *point;
    Node *next;
};

class BlockQueue {
private:
    int size;
    int maxSize;
    Node *head;
    Node *tail;

    pthread_cond_t pthreadCond;
    pthread_mutex_t pthreadMutex;
public:
    BlockQueue();

    ~BlockQueue();

    void *poll();

    void pull(void *any);
};

#endif //FFMPEGDOME_IBLOCKQUEUE_H
