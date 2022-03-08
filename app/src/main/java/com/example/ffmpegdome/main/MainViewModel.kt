package com.example.ffmpegdome.main

import android.content.Context
import android.content.Intent
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.example.ffmpegdome.configuration.ConfigurationActivity
import com.example.ffmpegdome.opensles.OpenSLESActivity
import com.example.ffmpegdome.onlyVideo.OnlyVideoActivity
import com.example.ffmpegdome.onlyAudio.OnlyAudioActivity

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
            add(ItemData("仅播放MP4中的视频", OnlyVideoActivity::class.java))
            add(ItemData("使用OpenSLES播放url(url可能失效)", OpenSLESActivity::class.java))
            add(ItemData("仅播放MP4中的音频", OnlyAudioActivity::class.java))
        }
    }

    fun gotoClazz(context: Context, clazz: Class<*>) {
        context.startActivity(Intent(context, clazz))
    }

    class ItemData(val title: String, val clazz: Class<*>)
}