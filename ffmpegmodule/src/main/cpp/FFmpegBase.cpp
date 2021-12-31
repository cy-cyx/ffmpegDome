//
// Created by JOYY on 2021/12/31.
//
#include "include/IFFmpegBase.h"

jstring getAvCodecConfiguration(JNIEnv *env) {
    char info[10000] = {0};
    sprintf(info, "%s\n", avcodec_configuration());
    return (*env).NewStringUTF(info);
}