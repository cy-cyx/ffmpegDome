//
// Created by JOYY on 2022/1/11.
//
// 解码
//
#include "decode/IVideoDecode.h"
#include "decode/IAudioDecode.h"
#include "render/IAudioRender.h"
#include "render/IVideoRender.h"
#include "sync/ISyncController.h"

#ifndef FFMPEGDOME_IFFMPEGDECODE_H
#define FFMPEGDOME_IFFMPEGDECODE_H

void playOnlyVideoTest(JNIEnv *env, jobject player, jstring uri, jobject surface);

void playOnlyAudioTest(JNIEnv *env, jclass clazz, jstring uri);

#endif //FFMPEGDOME_IFFMPEGDECODE_H
