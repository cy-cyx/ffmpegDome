//
// Created by JOYY on 2022/3/4.
//
#include <jni.h>
#include "ILog.h"

#ifndef FFMPEGDOME_C2JPOSTEVENT_H
#define FFMPEGDOME_C2JPOSTEVENT_H

#define EVENT_VIDEO_SIZE 1 // 回调视频宽高

extern jclass eventReceiver;
extern JavaVM *javaVm;

void postEventToJava(JNIEnv *env, jobject player, int what, int arg1, int arg2, jobject obj);

#endif //FFMPEGDOME_C2JPOSTEVENT_H