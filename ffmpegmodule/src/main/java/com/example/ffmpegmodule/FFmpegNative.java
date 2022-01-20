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

    public static int PLAYSTATE_STOPPED = 1;
    public static int PLAYSTATE_PAUSED = 2;
    public static int PLAYSTATE_PLAYING = 3;

    public native static String getAvCodecConfigurationNative();

    public native static void playNative(String url, Surface surface);

    public native static long createUrlAudioPlayer();

    public native static void destroyUrlAudioPlayer(long ptr);

    public native static void initUrlAudioPlayer(long ptr, String url);

    public native static void urlAudioPlayerPlay(long ptr);

    public native static int urlAudioPlayerGetState(long ptr);

    public native static void playVideoOfAudioNative(String url);
}
