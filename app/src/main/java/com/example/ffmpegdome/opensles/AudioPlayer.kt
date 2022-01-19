package com.example.ffmpegdome.opensles

import com.example.ffmpegmodule.FFmpegNative

/**
 * Time:2022/1/13 16:46
 * Author:cyx
 * Description:语音播放器
 */
class AudioPlayer {

    var audioNativePtr: Long = -1

    init {
        audioNativePtr = FFmpegNative.createUrlAudioPlayer()
    }

    fun init(url: String) {
        FFmpegNative.initUrlAudioPlayer(audioNativePtr, url)
    }

    fun play() {
        FFmpegNative.urlAudioPlayerPlay(audioNativePtr)
    }

    fun release() {
        if (audioNativePtr != -1L) {
            FFmpegNative.destroyUrlAudioPlayer(audioNativePtr)
            audioNativePtr = -1L
        }
    }

    fun getState(): Int {
        return FFmpegNative.urlAudioPlayerGetState(audioNativePtr)
    }
}