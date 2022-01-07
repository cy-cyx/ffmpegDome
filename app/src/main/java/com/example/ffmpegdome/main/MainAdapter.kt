package com.example.ffmpegdome.main

import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.recyclerview.widget.RecyclerView

/**
 * Time:2022/1/7 16:50
 * Author:
 * Description:
 */
class MainAdapter : RecyclerView.Adapter<RecyclerView.ViewHolder>() {

    var data = ArrayList<MainViewModel.ItemData>()
    var clickListen: ((Class<*>) -> Unit)? = null

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        return MainViewHolder(Button(parent.context))
    }

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
        val item = data[position]

        val button = (holder as? MainViewHolder)?.itemView as? Button
        button?.let {
            it.text = item.title
            it.setOnClickListener {
                clickListen?.invoke(item.clazz)
            }
        }
    }

    override fun getItemCount() = data.size

    class MainViewHolder(view: View) : RecyclerView.ViewHolder(view) {

    }
}