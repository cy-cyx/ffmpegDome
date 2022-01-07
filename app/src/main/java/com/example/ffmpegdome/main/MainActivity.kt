package com.example.ffmpegdome.main

import android.annotation.SuppressLint
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.ViewModelStore
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.ffmpegdome.R
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
    }
}