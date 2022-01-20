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
}

#ifndef FFMPEGDOME_IFFMPEGDECODE_H
#define FFMPEGDOME_IFFMPEGDECODE_H

typedef struct {
    const char *url;
    ANativeWindow *window;
} arg_decode;

void playVideoTest(JNIEnv *env, jclass clazz, jstring uri, jobject surface);

void playVideoOfAudio(JNIEnv *env, jclass clazz, jstring uri);

#endif //FFMPEGDOME_IFFMPEGDECODE_H
