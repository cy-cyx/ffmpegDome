//
// Created by JOYY on 2022/1/20.
//
#include "IFFmpegDecode.h"

void playVideoOfAudio(JNIEnv *env, jclass clazz, jstring uri) {
    const char *file_name = env->GetStringUTFChars(uri, NULL);

    LOGI("decode", "播放视频中的音频 %s", file_name);

    // 开一条线程解码，一条播放

}

