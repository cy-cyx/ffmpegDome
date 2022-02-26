//
// Created by JOYY on 2022/1/11.
//
#include "IFFmpegDecode.h"

void playVideoTest(JNIEnv *env, jclass clazz, jstring uri, jobject surface) {

    const char *file_name = env->GetStringUTFChars(uri, NULL);

    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (!nativeWindow) {
        LOGE("decode", "ANativeWindow 获取错误");
    }

    fm_VideoRender *fmVideoRender = new fm_VideoRender();
    fmVideoRender->initRender();
    fmVideoRender->nativeWindow = nativeWindow;
    fmVideoRender->execute();

    fm_VideoDecode *fmVideoDecode = new fm_VideoDecode();
    fmVideoDecode->initDecode();
    fmVideoDecode->videoRender = fmVideoRender;
    fmVideoDecode->execute(file_name);

//    time_t now = time(NULL);
//    LOGE("time", "%ld", now);
}

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
