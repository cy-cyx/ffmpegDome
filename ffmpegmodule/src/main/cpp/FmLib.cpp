#include <jni.h>

#include "include/ILog.h"
#include "include/IFFmpegBase.h"

/**
* Created by JOYY on 2021/12/31.
 *
 * 所有面对java层的接口都定义在这里
*/
jstring _getAvCodecConfiguration(JNIEnv *env, jclass clazz) {
    return getAvCodecConfiguration(env);
}

int registerNativeMethods(JNIEnv *env) {

    JNINativeMethod methods[] = {
            {"getAvCodecConfigurationNative", "()Ljava/lang/String;", (void *) _getAvCodecConfiguration},
    };
    const char *className = "com/example/ffmpegmodule/FFmpegNative";

    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
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