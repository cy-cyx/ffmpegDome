//
// Created by JOYY on 2022/2/28.
//

#ifndef FFMPEGDOME_ISYNC_H
#define FFMPEGDOME_ISYNC_H

class fm_ISyncController {
public:
    virtual long videoTimeStampDiff() = 0;

    virtual long audioTimeStampDiff() = 0;

    long getMonoTime();
};

#endif //FFMPEGDOME_ISYNC_H
