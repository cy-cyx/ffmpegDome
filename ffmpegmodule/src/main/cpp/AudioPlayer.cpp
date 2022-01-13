//
// Created by JOYY on 2022/1/13.
//
#include "IAudioPlayer.h"

long createAudioPlayer() {
    AudioPlayerInfo *playInfo = new AudioPlayerInfo();
    playInfo->engineObject = NULL;
    return (long) playInfo;
}

void destroyAudioPlayer(long playPtr) {
    AudioPlayerInfo *playInfo = (AudioPlayerInfo *) playPtr;
    (void) playInfo;
}