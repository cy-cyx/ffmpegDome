//
// Created by JOYY on 2022/1/13.
//

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "ILog.h"
#include <string.h>

#ifndef FFMPEGDOME_IAUDIOPLAYER_H
#define FFMPEGDOME_IAUDIOPLAYER_H

class fm_UrlAudioPlayer {

public:
    SLObjectItf engineObject;
    SLEngineItf engineEngine;
    SLObjectItf outputMixObject;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb;
    SLObjectItf uriPlayerObject;
    SLPlayItf uriPlayerPlay;

    long create();

    void destroy();

    void init(JNIEnv *env, jstring url);

    void play();

    unsigned int getState();

    static void urlPlayCallback(SLPlayItf caller, void *pContext, SLuint32 event);

private:
    // 输出混合的辅助效果，由缓冲队列播放器使用
    const SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
};


#endif //FFMPEGDOME_IAUDIOPLAYER_H
