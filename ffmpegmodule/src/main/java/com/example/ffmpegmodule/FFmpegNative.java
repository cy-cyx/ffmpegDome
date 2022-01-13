package com.example.ffmpegmodule;

import android.view.Surface;

/**
 * Time:2021/12/31 14:11
 * Author:
 * Description:
 */
public class FFmpegNative {

    static {
        System.loadLibrary("fmlib");
    }

    public native static String getAvCodecConfigurationNative();

    public native static void playNative(String url, Surface surface);

    public native static long createAudioPlayer();

    public native static void destroyAudioPlayer(long ptr);
}
