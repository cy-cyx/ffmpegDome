package com.example.ffmpegdome.onlyVideo

import android.app.Activity
import android.view.Surface
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.chat.albumlib.AlbumActivity
import com.chat.albumlib.AlbumControl

/**
 * Time:2022/1/10 17:55
 * Author:
 * Description:
 */
class OnlyVideoViewModel : ViewModel() {

    val videoUrl: MutableLiveData<String> = MutableLiveData()
    var onlyVideoPlayer: OnlyVideoPlayer? = null

    fun initData() {
        onlyVideoPlayer = OnlyVideoPlayer()
    }

    fun openAlbum(activity: Activity) {
        AlbumActivity.start(activity, arrayListOf(), AlbumControl.VIDEO, 1, 1000)
    }

    fun play(url: String, surface: Surface) {
        onlyVideoPlayer?.play(url, surface)
    }
}