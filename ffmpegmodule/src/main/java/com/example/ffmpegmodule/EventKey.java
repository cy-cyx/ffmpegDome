package com.example.ffmpegmodule;

/**
 * Time:2022/3/7 17:41
 * Author:
 * Description:用于接受c层回调的字段
 */
public enum EventKey {
    EVENT_VIDEO_SIZE(1),
    ;

    int key;

    EventKey(int i) {
        key = i;
    }

    public int getKey() {
        return key;
    }
}
