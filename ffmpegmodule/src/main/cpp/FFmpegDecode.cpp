//
// Created by JOYY on 2022/1/11.
//
#include "IFFmpegDecode.h"

void playVideoTest(JNIEnv *env, jobject player, jstring uri, jobject surface) {

    const char *file_name = env->GetStringUTFChars(uri, NULL);

    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (!nativeWindow) {
        LOGE("decode", "ANativeWindow 获取错误");
    }

    fm_SyncController1 *syncController = new fm_SyncController1();

    JavaVM *javaVm;
    env->GetJavaVM(&javaVm);

    fm_VideoRender *fmVideoRender = new fm_VideoRender();
    fmVideoRender->initRender();
    fmVideoRender->syncController = syncController;
    fmVideoRender->nativeWindow = nativeWindow;
    fmVideoRender->player = env->NewGlobalRef(player);
    fmVideoRender->execute();

    fm_VideoDecode *fmVideoDecode = new fm_VideoDecode();
    fmVideoDecode->initDecode();
    fmVideoDecode->videoRender = fmVideoRender;
    fmVideoDecode->player = env->NewGlobalRef(player);
    fmVideoDecode->execute(file_name);

    syncController->startPlay();
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
