package com.example.ffmpegdome.player

import android.app.Activity
import android.view.Surface
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.chat.albumlib.AlbumActivity
import com.chat.albumlib.AlbumControl
import com.example.ffmpegmodule.FFmpegNative

/**
 * Time:2022/1/10 17:55
 * Author:
 * Description:
 */
class PlayerViewModel : ViewModel() {

    val videoUrl: MutableLiveData<String> = MutableLiveData()

    fun openAlbum(activity: Activity) {
        AlbumActivity.start(activity, arrayListOf(), AlbumControl.VIDEO, 1, 1000)
    }

    fun play(url: String, surface: Surface) {
        FFmpegNative.playNative(url, surface)
    }
}