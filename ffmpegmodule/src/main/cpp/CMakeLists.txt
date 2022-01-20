cmake_minimum_required(VERSION 3.10.1)
project(Fmlib)

set(ffmpeg_lib_dir ${CMAKE_SOURCE_DIR}/../libs/${ANDROID_ABI})
set(ffmpeg_head_dir ${CMAKE_SOURCE_DIR}/ffmpeg/include)

include_directories(${ffmpeg_head_dir})

add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavutil.so)

add_library(swresample
        SHARED
        IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswresample.so)

add_library(avcodec
        SHARED
        IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavcodec.so)

add_library(avfilter
        SHARED
        IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavfilter.so)

add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswscale.so)

add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavformat.so)

#C 日志 ndk官方库
find_library(log-lib log)

#android ndk官方库
find_library(android-lib android)

#OpenSLES
find_library(openSLES-lib OpenSLES)

set(head_dir ${CMAKE_SOURCE_DIR}/include)
include_directories(${head_dir})

add_library(
        fmlib

        SHARED

        Fmlib.cpp
        FFmpegBase.cpp
        FFmpegDecode.cpp
        UrlAudioPlayer.cpp
        BlockQueue.cpp
        FFmpegDecode1.cpp
)


# 指定编译目标库时，cmake要链接的库
target_link_libraries(
        fmlib

        avutil
        swresample
        avcodec
        avfilter
        swscale
        avformat
        ${log-lib}
        ${android-lib}
        ${openSLES-lib}
)