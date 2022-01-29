//
// Created by JOYY on 2022/1/29.
//

#include <jni.h>
#include <string.h>
#include <pthread.h>

#include "./render/IAudioRender.h"

#include "ILog.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#ifndef FFMPEGDOME_IAUDIODECODE_H
#define FFMPEGDOME_IAUDIODECODE_H

class fm_AudioDecode {

public:
    fm_AudioRender *render;

    void initDecode();

    void execute(const char *fileName);

    void releaseDecode();

private:
    // 重采样的输出参数
    const static int outSwrChannel = 2;
    const static int outSwrChannelLayout = AV_CH_LAYOUT_STEREO;
    const static AVSampleFormat outSwrSampleFormat = AV_SAMPLE_FMT_S16;
    const static int outSwrSampleRate = 44100;
    const static int ACC_NB_SAMPLES = 1024; // acc是一帧1024

    const char *url;
    pthread_t threadPrt;

    static void *decodeTask(void *decode);

    static AVSampleFormat getAVSampleFormatByFormat(int format);
};

#endif //FFMPEGDOME_IAUDIODECODE_H
