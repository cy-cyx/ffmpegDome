//
// Created by JOYY on 2022/1/11.
//
// 解码
//
#include "decode/IVideoDecode.h"
#include "decode/IAudioDecode.h"
#include "render/IAudioRender.h"
#include "render/IVideoRender.h"

#ifndef FFMPEGDOME_IFFMPEGDECODE_H
#define FFMPEGDOME_IFFMPEGDECODE_H

void playVideoTest(JNIEnv *env, jclass clazz, jstring uri, jobject surface);

void playVideoOfAudio(JNIEnv *env, jclass clazz, jstring uri);

#endif //FFMPEGDOME_IFFMPEGDECODE_H
