//
// Created by JOYY on 2022/1/29.
//

#include <jni.h>

#include "ILog.h"
#include "IBlockQueue.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#ifndef FFMPEGDOME_IAUDIORENDER_H
#define FFMPEGDOME_IAUDIORENDER_H

typedef struct {
    uint8_t *frameData;
    int size;
} fm_AudioFrame;

class fm_AudioRender {
public:
    SLAndroidSimpleBufferQueueItf bufferQueue;

    void initRender();

    //存在阻塞
    void pullFrame(uint8_t *frameData, int size);

    void releaseRender();

private:
    int queueSize;
    BlockQueue *blockQueue;

    SLObjectItf engineObject;
    SLEngineItf engineEngine;
    SLObjectItf outputMixObject;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb;

    SLObjectItf playerObject;
    SLPlayItf uriPlayerPlay;

    static void queueCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context);
};

#endif //FFMPEGDOME_IAUDIORENDER_H
