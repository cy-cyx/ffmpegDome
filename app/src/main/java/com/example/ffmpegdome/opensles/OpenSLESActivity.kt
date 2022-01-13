package com.example.ffmpegdome.opensles

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.ffmpegdome.R

/**
 * Time:2022/1/13 16:39
 * Author:cyx
 * Description:openSLES播放音频
 */
class OpenSLESActivity : AppCompatActivity() {

    private var audioPlayer: AudioPlayer? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_opensles)
        audioPlayer = AudioPlayer()
    }

    override fun onDestroy() {
        super.onDestroy()
        audioPlayer?.release()
        audioPlayer = null
    }
}