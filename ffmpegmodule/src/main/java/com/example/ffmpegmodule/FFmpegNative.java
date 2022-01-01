package com.example.ffmpegmodule;

/**
 * Time:2021/12/31 14:11
 * Author:
 * Description:
 */
public class FFmpegNative {

    static {
        System.loadLibrary("fmlib");
    }

    public native static String getVersionNative();
}
