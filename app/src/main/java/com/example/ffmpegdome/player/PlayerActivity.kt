package com.example.ffmpegdome.player

import android.content.Intent
import android.os.Bundle
import android.view.Surface
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider
import com.chat.albumlib.AlbumActivity
import com.chat.albumlib.Image
import com.example.ffmpegdome.R
import kotlinx.android.synthetic.main.activity_player.*
import java.util.ArrayList

/**
 * Time:2022/1/10 17:36
 * Author:
 * Description:
 */
class PlayerActivity : AppCompatActivity() {

    private var viewModel: PlayerViewModel? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_player)
        viewModel = ViewModelProvider(this).get(PlayerViewModel::class.java)

        viewModel?.videoUrl?.observe(this) {
            tv_url.text = it
        }

        bn_album.setOnClickListener { viewModel?.openAlbum(this) }
        bn_play.setOnClickListener {
            val url = viewModel?.videoUrl?.value
            val surfaceTexture = ttv_screen.surfaceTexture
            if (url != null && surfaceTexture != null) {
                viewModel?.play(url, Surface(surfaceTexture))
            } else {
                Toast.makeText(this, "播放错误", Toast.LENGTH_SHORT).show()
            }
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == 1000) {
            val list = data?.getSerializableExtra(AlbumActivity.KEY_SELECT_DATA) as? ArrayList<Image> ?: ArrayList()
            list.getOrNull(0)?.let {
                viewModel?.videoUrl?.value = it.path
            }
        }
    }
}