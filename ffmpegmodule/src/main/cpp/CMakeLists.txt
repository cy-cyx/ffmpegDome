cmake_minimum_required(VERSION 3.10.1)
project(Fmlib)

#ffmpeg
set(fm_include ${CMAKE_SOURCE_DIR}/ffmpeg/include)
include_directories(${fm_include})
set(fm_lib ${CMAKE_SOURCE_DIR}/ffmpeg/lib/arm64-v8a)
link_directories(${fm_lib})

#find_library(LIBFM_PATH ffmpeg ${CMAKE_SOURCE_DIR}/ffmpeg/lib/arm64-v8a)
#IF (NOT LIBFM_PATH)
#    message("xxx")
#ENDIF (NOT LIBFM_PATH)

add_library(fmlib SHARED Fmlib.cpp)
#target_link_libraries(fmlib ${ffmpeg})
