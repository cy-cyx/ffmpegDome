package com.example.ffmpegdome.opensles

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.ffmpegdome.R
import com.example.ffmpegmodule.FFmpegNative
import kotlinx.android.synthetic.main.activity_opensles.*

/**
 * Time:2022/1/13 16:39
 * Author:cyx
 * Description:openSLES播放音频
 *
 * 测试的外链地址
 * https://music.xf1433.com/?name=%E5%8F%B9&type=kugou
 */
class OpenSLESActivity : AppCompatActivity() {

    private var mUrlAudioPlayer: UrlAudioPlayer? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_opensles)
        mUrlAudioPlayer = UrlAudioPlayer()

        bn_init.setOnClickListener {
            et_url.text.toString().let {
                mUrlAudioPlayer?.init(it)
            }
        }

        bn_play.setOnClickListener {
            mUrlAudioPlayer?.play()
        }

        bn_state.setOnClickListener {
            val state = mUrlAudioPlayer?.getState()
            state?.let {
                when (it) {
                    FFmpegNative.PLAYSTATE_STOPPED -> tv_state.text = "初始化"
                    FFmpegNative.PLAYSTATE_PLAYING -> tv_state.text = "播放中"
                    FFmpegNative.PLAYSTATE_PAUSED -> tv_state.text = "暂停（播放结束）"
                }
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        mUrlAudioPlayer?.release()
        mUrlAudioPlayer = null
    }
}