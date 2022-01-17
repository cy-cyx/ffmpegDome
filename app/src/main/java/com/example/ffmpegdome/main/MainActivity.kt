package com.example.ffmpegdome.main

import android.Manifest
import android.annotation.SuppressLint
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelStore
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.chat.albumlib.AlbumActivity
import com.chat.albumlib.AlbumControl
import com.example.ffmpegdome.R
import com.example.ffmpegdome.utils.PermissionUtil
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    val TAG = "MainActivity"

    private var viewModel: MainViewModel? = null
    private var mainAdapter: MainAdapter? = null

    @SuppressLint("NotifyDataSetChanged")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        viewModel = ViewModelProvider(this).get(MainViewModel::class.java)

        mainAdapter = MainAdapter().apply {
            clickListen = {
                viewModel?.gotoClazz(this@MainActivity, it)
            }
        }

        rv_list.run {
            adapter = mainAdapter
            layoutManager = LinearLayoutManager(this@MainActivity)
        }

        viewModel?.data?.observe(this) {
            mainAdapter?.data = it
            mainAdapter?.notifyDataSetChanged()
        }

        // 申请一些必要的权限
        PermissionUtil.requestRuntimePermissions(this, arrayOf(
            Manifest.permission.MODIFY_AUDIO_SETTINGS // opensl 播放音频需要
        ), object : PermissionUtil.IPermissionCallback {
            override fun nextStep() {
            }

            override fun cancel() {
                finish()
            }
        })
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        PermissionUtil.onRequestPermissionsResult(this, requestCode, permissions, grantResults)
    }
}