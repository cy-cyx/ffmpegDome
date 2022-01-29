//
// Created by JOYY on 2022/1/20.
//
#include "IFFmpegDecode.h"

#include "./decode/IAudioDecode.h"
#include "./render/IAudioRender.h"

void playVideoOfAudio(JNIEnv *env, jclass clazz, jstring uri) {

    const char *file_name = env->GetStringUTFChars(uri, NULL);
    LOGI("decode", "播放视频中的音频 %s", file_name);

    // 音频播放器
    fm_AudioRender *render = new fm_AudioRender();
    render->initRender();

    // 音频解码器
    fm_AudioDecode *decode = new fm_AudioDecode();
    decode->initDecode();
    decode->render = render;
    decode->execute(file_name);
}

