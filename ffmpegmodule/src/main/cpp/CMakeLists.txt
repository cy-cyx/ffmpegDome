cmake_minimum_required(VERSION 3.10.1)
project(Fmlib)

#ffmpeg
set(fm_include_avcodec ${CMAKE_SOURCE_DIR}/ffmpeg/include/libavcodec)
set(fm_include_avfilter ${CMAKE_SOURCE_DIR}/ffmpeg/include/libavfilter)
set(fm_include_avformat ${CMAKE_SOURCE_DIR}/ffmpeg/include/libavformat)
set(fm_include_avutil ${CMAKE_SOURCE_DIR}/ffmpeg/include/libavutil)
set(fm_include_swresample ${CMAKE_SOURCE_DIR}/ffmpeg/include/libswresample)
set(fm_include_swscale ${CMAKE_SOURCE_DIR}/ffmpeg/include/libswscale)
include_directories(${fm_include_avcodec})
include_directories(${fm_include_avfilter})
include_directories(${fm_include_avformat})
include_directories(${fm_include_avutil})
include_directories(${fm_include_swresample})
include_directories(${fm_include_swscale})

set(fm_lib ${CMAKE_SOURCE_DIR}/ffmpeg/lib/arm64-v8a)
link_directories(${fm_lib})

# 关联库第二种写法
#find_library(LIBFM_PATH ffmpeg ${CMAKE_SOURCE_DIR}/ffmpeg/lib/arm64-v8a)
#IF (NOT LIBFM_PATH)
#    message("xxx")
#ENDIF (NOT LIBFM_PATH)
#target_link_libraries(fmlib ${ffmpeg})

#include
set(include ${CMAKE_SOURCE_DIR}/include)

add_library(fmlib SHARED Fmlib.cpp FFmpegBase.cpp)