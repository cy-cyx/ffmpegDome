#include <jni.h>

#include "include/ILog.h"
#include "include/IFFmpegBase.h"
#include "include/IFFmpegDecode.h"
#include "include/IUrlAudioPlayer.h"

/**
* Created by JOYY on 2021/12/31.
 *
 * 所有面对java层的接口都定义在这里
*/
void _playVideoTest(JNIEnv *env, jclass clazz, jobject player, jstring uri, jobject surface) {
    return playVideoTest(env, player, uri, surface);
}

jstring _getAvCodecConfiguration(JNIEnv *env, jclass clazz) {
    return fm_getAvCodecConfiguration(env);
}

long _createUrlAudioPlayer(JNIEnv *env, jclass clazz) {
    return fm_createUrlAudioPlayer();
}

void _destroyUrlAudioPlayer(JNIEnv *env, jclass clazz, long playPtr) {
    fm_destroyUrlAudioPlayer(playPtr);
}

void _initUrlAudioPlayer(JNIEnv *env, jclass clazz, long playPtr, jstring url) {
    fm_initUrlAudioPlayer(env, playPtr, url);
}

void _urlAudioPlayerPlay(JNIEnv *env, jclass clazz, long playPtr) {
    fm_urlAudioPlayerPlay(playPtr);
}

int _urlAudioPlayerGetState(JNIEnv *env, jclass clazz, long playPtr) {
    return (int) fm_urlAudioPlayerGetState(playPtr);
}

void _playVideoOfAudio(JNIEnv *env, jclass clazz, jstring uri) {
    playVideoOfAudio(env, clazz, uri);
}

/**
 * 动态注册
 */
int registerNativeMethods(JNIEnv *env) {

    JNINativeMethod methods[] = {
            {"getAvCodecConfigurationNative", "()Ljava/lang/String;",                                                          (void *) _getAvCodecConfiguration},
            {"playNative",                    "(Lcom/example/ffmpegmodule/IPlayer;Ljava/lang/String;Landroid/view/Surface;)V", (void *) _playVideoTest},
            {"createUrlAudioPlayer",          "()J",                                                                           (void *) _createUrlAudioPlayer},
            {"destroyUrlAudioPlayer",         "(J)V",                                                                          (void *) _destroyUrlAudioPlayer},
            {"initUrlAudioPlayer",            "(JLjava/lang/String;)V",                                                        (void *) _initUrlAudioPlayer},
            {"urlAudioPlayerPlay",            "(J)V",                                                                          (void *) _urlAudioPlayerPlay},
            {"urlAudioPlayerGetState",        "(J)I",                                                                          (void *) _urlAudioPlayerGetState},
            {"playVideoOfAudioNative",        "(Ljava/lang/String;)V",                                                         (void *) _playVideoOfAudio}
    };
    const char *className = "com/example/ffmpegmodule/FFmpegNative";

    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }

    // FindClass是局部引用需要转成全局引用
    LOGI("postEvent", "获得局部clazz，转成全局");
    env->GetJavaVM(&javaVm);
    eventReceiver = static_cast<jclass>((jobject) env->NewGlobalRef(clazz));

    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

// System.loadLibrary 调用后的回调
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    if (env == NULL) {
        return -1;
    }
    // 动态注册
    if (!registerNativeMethods(env)) {
        return -1;
    }
    return JNI_VERSION_1_6;
}