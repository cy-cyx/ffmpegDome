//
// Created by JOYY on 2022/3/8.
//
#include "include/C2JPostEvent.h"

jclass eventReceiver = NULL;

JavaVM* javaVm;

void postEventToJava(JNIEnv *env, jobject player, int what, int arg1, int arg2, jobject obj) {

    if (env == NULL) {
        LOGE("postEvent", "env 为空");
        return;
    }
    if (eventReceiver == NULL) {
        LOGE("postEvent", "clazz 为空");
        return;
    }
    jmethodID method = env->GetStaticMethodID(eventReceiver, "postEventFromNative",
                                              "(Ljava/lang/Object;IIILjava/lang/Object;)V");
    env->CallStaticVoidMethod(eventReceiver, method, player, what, arg1, arg2, obj);
}
