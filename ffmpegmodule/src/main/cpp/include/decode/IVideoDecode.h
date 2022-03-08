//
// Created by 小新 on 2022/2/13.
//

#include <pthread.h>
#include "ILog.h"
#include "jni.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#include "render/IVideoRender.h"

#ifndef FFMPEGDOME_IVIDEODECODE_H
#define FFMPEGDOME_IVIDEODECODE_H

class fm_VideoDecode {
public:
    jobject player;

    fm_VideoRender *videoRender;

    void initDecode();

    void execute(const char *fileName);

    void releaseDecode();

private:
    const char *url;
    pthread_t threadPrt;

    static void *decodeTask(void *decode);
};

#endif //FFMPEGDOME_IVIDEODECODE_H
