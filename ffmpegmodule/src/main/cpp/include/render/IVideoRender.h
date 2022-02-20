//
// Created by 小新 on 2022/2/13.
//
#include <pthread.h>
#include "ILog.h"
#include <IBlockQueue.h>

#include <android/native_window.h>
#include <android/native_window_jni.h>

extern "C" {
#include <libavformat/avformat.h>
}

#ifndef FFMPEGDOME_IVIDEORENDER_H
#define FFMPEGDOME_IVIDEORENDER_H

typedef struct {
    AVFrame *avFrame;
    double timestamp;
    int width;
    int height;
} fm_VideoFrame;

class fm_VideoRender {
public:
    ANativeWindow *nativeWindow;

    void initRender();

    void execute();

    //存在阻塞
    void pullFrame(AVFrame *avFrame, double timestamp, int width, int height);

    void releaseRender();

    void setBuffersGeometry(int32_t width, int32_t height, int32_t format);

private:
    BlockQueue *blockQueue;

    pthread_t threadPrt;

    [[noreturn]] static void *renderTask(void *render);
};

#endif //FFMPEGDOME_IVIDEORENDER_H
