//
// Created by JOYY on 2022/1/11.
//
// 视频软解
//
#include <jni.h>
#include <string.h>
#include <pthread.h>

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "ILog.h"
#include "IBlockQueue.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#ifndef FFMPEGDOME_IFFMPEGDECODE_H
#define FFMPEGDOME_IFFMPEGDECODE_H

// 重采样的输出参数
const int outChannel = 2;
const AVSampleFormat outSampleFormat = AV_SAMPLE_FMT_S16P;
const int outSampleRate = 44100;

typedef struct {
    const char *url;
    ANativeWindow *window;
} arg_decode;

typedef struct {
    const char *url;
    BlockQueue *queue;
} AudioDecodeInfo;

void playVideoTest(JNIEnv *env, jclass clazz, jstring uri, jobject surface);

void playVideoOfAudio(JNIEnv *env, jclass clazz, jstring uri);

#endif //FFMPEGDOME_IFFMPEGDECODE_H
