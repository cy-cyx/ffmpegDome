//
// Created by JOYY on 2022/1/13.
//

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "ILog.h"
#include <string.h>

#ifndef FFMPEGDOME_IAUDIOPLAYER_H
#define FFMPEGDOME_IAUDIOPLAYER_H

typedef struct {
    SLObjectItf engineObject;
    SLEngineItf engineEngine;
    SLObjectItf outputMixObject;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb;

    // uri
    SLObjectItf uriPlayerObject;
    SLPlayItf uriPlayerPlay;
} fm_UrlAudioPlayerInfo;

// 输出混合的辅助效果，由缓冲队列播放器使用
static const SLEnvironmentalReverbSettings reverbSettings =
        SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

long fm_createUrlAudioPlayer();

void fm_destroyUrlAudioPlayer(long playPtr);

void fm_initUrlAudioPlayer(JNIEnv *env, long playPtr, jstring url);

void fm_urlAudioPlayerPlay(long playPtr);

unsigned int fm_urlAudioPlayerGetState(long playPtr);

#endif //FFMPEGDOME_IAUDIOPLAYER_H
