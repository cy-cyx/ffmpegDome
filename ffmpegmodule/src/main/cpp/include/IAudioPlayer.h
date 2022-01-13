//
// Created by JOYY on 2022/1/13.
//

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "ILog.h"

#ifndef FFMPEGDOME_IAUDIOPLAYER_H
#define FFMPEGDOME_IAUDIOPLAYER_H

typedef struct {
    SLObjectItf engineObject;
} AudioPlayerInfo;

long createAudioPlayer();

void destroyAudioPlayer(long playPtr);

#endif //FFMPEGDOME_IAUDIOPLAYER_H
