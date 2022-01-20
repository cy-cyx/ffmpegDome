package com.example.ffmpegdome.main

import android.content.Context
import android.content.Intent
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.example.ffmpegdome.configuration.ConfigurationActivity
import com.example.ffmpegdome.opensles.OpenSLESActivity
import com.example.ffmpegdome.player.PlayerActivity
import com.example.ffmpegdome.playvideoofaudio.PlayVideoOfAudioActivity

/**
 * Time:2022/1/7 16:25
 * Author:
 * Description:
 */
class MainViewModel : ViewModel() {

    var data: MutableLiveData<ArrayList<ItemData>> = MutableLiveData()

    init {
        data.value = ArrayList<ItemData>().apply {
            add(ItemData("so编译参数", ConfigurationActivity::class.java))
            add(ItemData("软解播放", PlayerActivity::class.java))
            add(ItemData("使用OpenSLES播放url", OpenSLESActivity::class.java))
            add(ItemData("播放视频中的音频", PlayVideoOfAudioActivity::class.java))
        }
    }

    fun gotoClazz(context: Context, clazz: Class<*>) {
        context.startActivity(Intent(context, clazz))
    }

    class ItemData(val title: String, val clazz: Class<*>)
}