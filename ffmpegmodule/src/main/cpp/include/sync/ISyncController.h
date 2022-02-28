//
// Created by 蔡翼翔 on 2022/2/20.
//

#include "sync/ISync.h"
#include "render/IAudioRender.h"
#include "render/IVideoRender.h"
#include "ILog.h"

#ifndef FFMPEGDOME_ISYNCCONTROLLER_H
#define FFMPEGDOME_ISYNCCONTROLLER_H

class fm_SyncController : public fm_ISyncController {

public:
    long audioTimeStampDiff();

    long videoTimeStampDiff();

    void startPlay();

private:
    long startTp = 0;
};

#endif //FFMPEGDOME_ISYNCCONTROLLER_H
