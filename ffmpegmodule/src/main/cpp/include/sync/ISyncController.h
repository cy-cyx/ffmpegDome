//
// Created by 蔡翼翔 on 2022/2/20.
//

#include "render/IAudioRender.h"
#include "render/IVideoRender.h"
#include "time.h"

#ifndef FFMPEGDOME_ISYNCCONTROLLER_H
#define FFMPEGDOME_ISYNCCONTROLLER_H

class fm_ISyncController {
public:
    virtual double videoTimeStampDiff() = 0;

    virtual double audioTimeStampDiff() = 0;

private:
    fm_AudioRender audioRender;
    fm_VideoRender videoRender;
};

class fm_SyncController : fm_ISyncController {

public:
    double audioTimeStampDiff();

    double videoTimeStampDiff();
};

#endif //FFMPEGDOME_ISYNCCONTROLLER_H
