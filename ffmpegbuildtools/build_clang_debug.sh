#!/bin/sh
# NDK 所在的路径
NDK=D:/androidsdk/ndk/21.4.7075529
# 需要编译出的平台，这里是
# arm64-v8a（aarch64）aarch64-linux-android- aarch64-linux-android
# armeabi-v7a（armv7a）arm-linux-androideabi- armv7a-linux-androideabi
# x86
# x86_84
ARCH=aarch64
# 支持的最低 Android API
API=21
# 编译后输出目录，在 ffmpeg 源码目录下的 /android/arm64-v8a
OUTPUT=${pwd}/android/arm64-v8a
# NDK 交叉编译工具链所在路径
TOOLCHAIN=D:/androidsdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/windows-x86_64

build()
{
make clean
./configure \
--target-os=android \
--prefix=$OUTPUT \
--arch=$ARCH \
--sysroot=$TOOLCHAIN/sysroot \
--enable-shared \
--disable-static \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-avdevice \
--enable-debug=3 \
--disable-stripping \
--disable-doc \
--enable-cross-compile \
--cross-prefix=$TOOLCHAIN/bin/aarch64-linux-android- \
--cc=$TOOLCHAIN/bin/aarch64-linux-android$API-clang \
--cxx=$TOOLCHAIN/bin/aarch64-linux-android$API-clang++ \
--extra-cflags="-fpic"
make clean
make -j12
make install
}

build