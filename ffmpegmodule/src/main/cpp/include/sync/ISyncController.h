//
// Created by 蔡翼翔 on 2022/2/20.
//

#include "sync/ISync.h"
#include "render/IAudioRender.h"
#include "render/IVideoRender.h"
#include "ILog.h"

#ifndef FFMPEGDOME_ISYNCCONTROLLER_H
#define FFMPEGDOME_ISYNCCONTROLLER_H

/**
 * 按照系统时钟同步
 */
class fm_SyncController1 : public fm_ISyncController {

public:
    long audioTimeStampDiff();

    long videoTimeStampDiff();

    void startPlay();

private:
    long startTp = 0;
};

class fm_SyncController2 : public fm_ISyncController {
public:
    long audioTimeStampDiff();

    long videoTimeStampDiff();

private:
    fm_AudioRender audioRender;
    fm_VideoRender videoRender;
};

#endif //FFMPEGDOME_ISYNCCONTROLLER_H
