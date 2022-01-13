package com.example.ffmpegdome.configuration

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.ffmpegdome.R
import com.example.ffmpegmodule.FFmpegNative
import kotlinx.android.synthetic.main.activity_config.*

/**
 * Time:2022/1/7 17:18
 * Author:cyx
 * Description:测试是否调通ffmpeg，打印交叉编译的信息
 */
class ConfigurationActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_config)
        tv_config.text = FFmpegNative.getAvCodecConfigurationNative().replace(" ", "\n")
    }
}