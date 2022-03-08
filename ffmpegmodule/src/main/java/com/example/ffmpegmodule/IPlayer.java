package com.example.ffmpegmodule;

/**
 * Time:2022/3/1 15:55
 * Author:cyx
 * Description:所有播放类都需要该接口，用于接受底层回调
 */
public interface IPlayer {
    void postEvent(int what, int arg1, int arg2, Object obj);
}
