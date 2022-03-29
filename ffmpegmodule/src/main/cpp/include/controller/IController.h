//
// Created by JOYY on 2022/3/29.
//

#ifndef FFMPEGDOME_ICONTROLLER_H
#define FFMPEGDOME_ICONTROLLER_H

#define MODULE_ONLY_AUDIO 0x0001 // 仅有使用音频
#define MODULE_ONLY_VIDEO 0x0010 // 仅有使用视频
#define MODULE_PLAYER 0x0011 // 音视频模式

#define MODULE_SIZE_ONLY_AUDIO 2
#define MODULE_SIZE_ONLY_VIDEO 2
#define MODULE_SIZE_PLAYER 4

/**
 * 对外开发接口
 * [fm_IController#play()]
 * [fm_IController#pause()]
 * [fm_IController#isPlaying()]
 * [fm_IController#setLoop(bool loop)]
 * [fm_IController#seek(int timestamp)]
 * [fm_IController#release()]
 */
class fm_IController {
public:
    fm_IController();

    int moduleType; // 当前控制器模式

    void play();  // 播放
    void pause(); // 暂停

    bool audioPlaying; // 音频是否播放中
    bool videoPlaying; // 视频是否播放中
    void isPlaying(); // 当前是否正在播放，

    bool audioIsEnd; // 音频是否到尾部（如果没有循环，到达尾部会自动暂停，仅有在音视频同时到尾部，点击play才有用）
    bool videoIsEnd; // 视频是否到尾部
    void endOfAudio();
    void endOfVideo();

    void loop;
    void setLoop(bool loop);

    bool isRelease; // 结束
    void release();  // 回收

    int moduleSize; // 总共有几个模块（支持单播放器）
    void endOfModule(); // 每个模块结束，通知一下控制端，做统一回收

    void seek(int timestamp); // seekTo
    int needSeekToTimeStamp;
    int audioNeedSeekTo; // 音频需要重新seekTo
    int videoNeedSeekTo; // 视频需要重新seekTo

    pthread_mutex_t pthreadMutex;
    pthread_cond_t audioPthreadCond; // 用于控制是否播放
    pthread_cond_t videoPthreadCond;
};

#endif //FFMPEGDOME_ICONTROLLER_H
