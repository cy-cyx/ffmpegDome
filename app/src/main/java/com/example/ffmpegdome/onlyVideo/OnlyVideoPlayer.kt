package com.example.ffmpegdome.onlyVideo

import android.os.Handler
import android.os.Looper
import android.os.Message
import android.view.Surface
import com.example.ffmpegmodule.EventKey
import com.example.ffmpegmodule.FFmpegNative
import com.example.ffmpegmodule.IPlayer

/**
 * Time:2022/3/7 17:32
 * Author:
 * Description:仅仅播放视频的播放管理
 */
class OnlyVideoPlayer : IPlayer {

    private val eventHandler = EventHandler(Looper.getMainLooper())

    var videoSizeChangeListen: onVideoSizeChangeListen? = null

    fun play(url: String, surface: Surface) {
        FFmpegNative.playNative(this, url, surface)
    }

    override fun postEvent(what: Int, arg1: Int, arg2: Int, obj: Any?) {
        val message = eventHandler.obtainMessage(what, arg1, arg2, obj)
        eventHandler.sendMessage(message)
    }

    private inner class EventHandler(looper: Looper) : Handler(looper) {
        override fun handleMessage(msg: Message) {
            when (msg.what) {
                EventKey.EVENT_VIDEO_SIZE.key -> {
                    videoSizeChangeListen?.onChange(msg.arg1, msg.arg2)
                }
            }
        }
    }

    interface onVideoSizeChangeListen {
        fun onChange(width: Int, height: Int)
    }
}