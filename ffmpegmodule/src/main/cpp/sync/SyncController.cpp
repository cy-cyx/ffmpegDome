//
// Created by 蔡翼翔 on 2022/2/20.
//

#include "sync/ISyncController.h"

long fm_ISyncController::getMonoTime() {
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000 + now.tv_nsec / 1000000;
}

void fm_SyncController::startPlay() {
    startTp = fm_ISyncController::getMonoTime();
    LOGI("sync", "开始播放");
}

long fm_SyncController::audioTimeStampDiff() {
    return 0.0;
}

long fm_SyncController::videoTimeStampDiff() {
    return fm_ISyncController::getMonoTime() - startTp;
}