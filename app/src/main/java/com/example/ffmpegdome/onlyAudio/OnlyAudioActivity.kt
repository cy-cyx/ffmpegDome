package com.example.ffmpegdome.onlyAudio

import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider
import com.chat.albumlib.AlbumActivity
import com.chat.albumlib.Image
import com.example.ffmpegdome.R
import kotlinx.android.synthetic.main.activity_video_of_audio.*
import java.util.ArrayList

/**
 * Time:2022/1/20 17:31
 * Author:
 * Description:
 */
class OnlyAudioActivity : AppCompatActivity() {

    private var mViewModel: OnlyAudioViewModel? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_only_audio)
        mViewModel = ViewModelProvider(this).get(OnlyAudioViewModel::class.java)

        mViewModel?.videoUrl?.observe(this, {
            tv_url.text = it
        })

        bn_album.setOnClickListener {
            mViewModel?.openAlbum(this)
        }

        bn_play_audio.setOnClickListener {
            mViewModel?.videoUrl?.value?.let {
                mViewModel?.openVideoOfAudio(it)
            } ?: kotlin.run {
                Toast.makeText(this, "先选择视频", Toast.LENGTH_SHORT).show()
            }
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == 1000) {
            val list = data?.getSerializableExtra(AlbumActivity.KEY_SELECT_DATA) as? ArrayList<Image> ?: ArrayList()
            list.getOrNull(0)?.let {
                mViewModel?.videoUrl?.value = it.path
            }
        }
    }
}