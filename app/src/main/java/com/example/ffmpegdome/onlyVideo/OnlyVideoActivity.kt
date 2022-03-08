package com.example.ffmpegdome.onlyVideo

import android.content.Intent
import android.graphics.Matrix
import android.graphics.SurfaceTexture
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.TextureView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider
import com.chat.albumlib.AlbumActivity
import com.chat.albumlib.Image
import com.example.ffmpegdome.R
import com.example.ffmpegdome.utils.ScreenUtils
import kotlinx.android.synthetic.main.activity_player.*
import java.util.ArrayList

/**
 * Time:2022/1/10 17:36
 * Author:cyx
 * Description:软解视频帧信息，渲染到surface
 */
class OnlyVideoActivity : AppCompatActivity() {

    private var mViewModel: OnlyVideoViewModel? = null
    private var curSurface: Surface? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_only_video)
        mViewModel = ViewModelProvider(this).get(OnlyVideoViewModel::class.java)
        mViewModel?.initData()

        mViewModel?.videoUrl?.observe(this) {
            tv_url.text = it
        }

        ttv_screen.surfaceTextureListener = object : TextureView.SurfaceTextureListener {
            override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
                curSurface = Surface(surface)
            }

            override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
            }

            override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
                curSurface = null
                return false
            }

            override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
            }
        }
        bn_album.setOnClickListener { mViewModel?.openAlbum(this) }
        bn_play.setOnClickListener {
            val url = mViewModel?.videoUrl?.value
            if (url != null && curSurface != null) {
                mViewModel?.play(url, curSurface!!)
            } else {
                Toast.makeText(this, "播放错误", Toast.LENGTH_SHORT).show()
            }
        }
        mViewModel?.onlyVideoPlayer?.videoSizeChangeListen = object : OnlyVideoPlayer.onVideoSizeChangeListen {
            override fun onChange(width: Int, height: Int) {
                Log.e("OnlyVideoActivity", "java接受到Native层的回调 宽：$width 高：$height")
                resizeTextureViewByMatrix(width, height)
            }
        }
    }

    // 设置Matrix让显示正常
    private fun resizeTextureViewByMatrix(width: Int, height: Int) {
        val matrix = Matrix()

        val viewWidth = ScreenUtils.getWidthPixels();
        val viewHeight = ScreenUtils.dp2px(200f)

        val pointX = viewWidth / 2f
        val pointY = viewHeight / 2f
        var scaleX = 1f
        var scaleY = 1f

        if (width / height.toFloat() >= viewWidth / viewHeight.toFloat()) {
            val tempHeight = viewWidth * height / width.toFloat()
            scaleY = tempHeight / viewHeight.toFloat()
        } else {
            val tempWidth = viewHeight * width / height.toFloat()
            scaleX = tempWidth / viewWidth.toFloat()
        }
        Log.d("OnlyVideoActivity", "scaleX=$scaleX scaleY=$scaleY pointX=$pointX pointY=$pointY")

        matrix.postScale(scaleX, scaleY, pointX, pointY)

        ttv_screen.setTransform(matrix)
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