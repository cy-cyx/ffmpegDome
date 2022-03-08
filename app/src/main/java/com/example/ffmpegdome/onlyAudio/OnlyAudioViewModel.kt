package com.example.ffmpegdome.onlyAudio

import android.app.Activity
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.chat.albumlib.AlbumActivity
import com.chat.albumlib.AlbumControl
import com.example.ffmpegmodule.FFmpegNative

/**
 * Time:2022/1/20 17:44
 * Author:
 * Description:
 */
class OnlyAudioViewModel : ViewModel() {

    val videoUrl: MutableLiveData<String> = MutableLiveData()

    fun openAlbum(activity: Activity) {
        AlbumActivity.start(activity, arrayListOf(), AlbumControl.VIDEO, 1, 1000)
    }

    fun openVideoOfAudio(url: String) {
        FFmpegNative.playVideoOfAudioNative(url)
    }
}