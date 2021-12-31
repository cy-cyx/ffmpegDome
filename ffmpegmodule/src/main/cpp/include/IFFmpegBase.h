/**
 * Created by JOYY on 2021/12/31.
 *
 * 用于输出一些基础的
*/

extern "C" {
#include ".././ffmpeg/include/libavcodec/avcodec.h"
}

#include <jni.h>
#include <string.h>

#ifndef FFMPEGDOME_IFFMPEGBASE_H

jstring getAvCodecConfiguration(JNIEnv *env);

#define FFMPEGDOME_IFFMPEGBASE_H

#endif
