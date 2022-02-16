//
// Created by 小新 on 2022/2/13.
//
#include "render/IVideoRender.h"

void fm_VideoRender::initRender() {
    blockQueue = new BlockQueue;
}

void fm_VideoRender::execute() {
    pthread_create(&threadPrt, NULL, fm_VideoRender::renderTask, this);
}

void *fm_VideoRender::renderTask(void *render) {
    fm_VideoRender *videoRender = (fm_VideoRender *) render;

    ANativeWindow_Buffer windowBuffer;

    while (true) {

        // lock native window buffer
        fm_VideoFrame *frame = (fm_VideoFrame *) videoRender->blockQueue->poll();

        LOGI("videoRender", "从队列拿出一帧开始画");

        ANativeWindow_lock(videoRender->nativeWindow, &windowBuffer, 0);

        // 获取stride
        uint8_t *dst = (uint8_t *) windowBuffer.bits;
        int dstStride = windowBuffer.stride * 4;
        uint8_t *src = frame->avFrame->data[0];
        int srcStride = frame->avFrame->linesize[0];

        // 由于window的stride和帧的stride不同,因此需要逐行复制
        int h;
        for (h = 0; h < frame->height; h++) {
            memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
        }
        ANativeWindow_unlockAndPost(videoRender->nativeWindow);

        av_free(frame->avFrame);
        free(frame);
    }

    videoRender->releaseRender();

    pthread_exit(NULL);
}

void fm_VideoRender::pullFrame(AVFrame *avFrame, int timestamp, int width, int height) {
    fm_VideoFrame *frame = new fm_VideoFrame();
    frame->avFrame = avFrame;
    frame->timestamp = timestamp;
    frame->width = width;
    frame->height = height;

    blockQueue->pull(frame);
}

void fm_VideoRender::releaseRender() {

}

void fm_VideoRender::setBuffersGeometry(int32_t width, int32_t height, int32_t format) {
    LOGI("render", "设置native window的buffer大小,可自动拉伸");
    ANativeWindow_setBuffersGeometry(nativeWindow, width, height,
                                     WINDOW_FORMAT_RGBA_8888);
}

