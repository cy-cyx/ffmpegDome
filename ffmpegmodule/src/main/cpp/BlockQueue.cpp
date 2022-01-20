//
// Created by JOYY on 2022/1/20.
//
#include <cstdlib>
#include <unistd.h>
#include "include/IBlockQueue.h"

BlockQueue::BlockQueue() {
    size = 0;
    maxSize = 10;
    head = NULL;
    tail = NULL;
    pthreadCond = PTHREAD_COND_INITIALIZER;
    pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_init(&pthreadCond, NULL);
    pthread_mutex_init(&pthreadMutex, NULL);
}

BlockQueue::~BlockQueue() {
    size = 0;
    head = NULL;
    tail = NULL;
    pthread_cond_signal(&pthreadCond);
    pthread_cond_destroy(&pthreadCond);

    pthread_mutex_destroy(&pthreadMutex);
}

void *BlockQueue::poll() {
    pthread_mutex_lock(&pthreadMutex);

    while (size < 1) {
        pthread_cond_wait(&pthreadCond, &pthreadMutex);
    }

    Node *target = head;
    head = head->next;

    if (target == tail) {
        tail = NULL;
    }

    size--;
    pthread_cond_signal(&pthreadCond);
    pthread_mutex_unlock(&pthreadMutex);

    void *result = target->point;
    free(target);
    return result;
}

void BlockQueue::pull(void *any) {
    Node *node = new Node();
    node->point = any;
    node->next = NULL;

    pthread_mutex_lock(&pthreadMutex);

    while (size > maxSize) {
        pthread_cond_wait(&pthreadCond, &pthreadMutex);
    }

    if (head == NULL) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    size++;
    pthread_cond_signal(&pthreadCond);
    pthread_mutex_unlock(&pthreadMutex);
}