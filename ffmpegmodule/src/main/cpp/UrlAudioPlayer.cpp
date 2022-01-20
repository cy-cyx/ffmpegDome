//
// Created by JOYY on 2022/1/13.
//
#include <cstdlib>
#include "IUrlAudioPlayer.h"

// 播放url无回调
void urlPlayCallback(SLPlayItf caller, void *pContext, SLuint32 event) {
    LOGI("audio", "播放回调 %d", event);
}

long createUrlAudioPlayer() {
    UrlAudioPlayerInfo *playInfo = new UrlAudioPlayerInfo();
    SLObjectItf engineObject;
    SLEngineItf engineEngine;
    SLObjectItf outputMixObject;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb;

    SLresult result;
    // 创建引擎
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "新建引擎失败");
    }
    // 初始化引擎
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "初始化引擎失败");
    }
    playInfo->engineObject = engineObject;

    // 获取引擎接口，这是创建其他对象所必需的
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "创建引擎接口失败");
    }

    playInfo->engineEngine = engineEngine;

    // 创建输出混音，环境混响指定为非必需接口
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "创建输出混音失败");
    }

    // 初始化混音接口
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "初始化混音失败");
    }

    playInfo->outputMixObject = outputMixObject;

    // 设置环境混响
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        if (SL_RESULT_SUCCESS != result) {
            LOGE("audio", "设置环境混音失败");
        }
    } else {
        LOGE("audio", "获得环境混音失败");
    }

    playInfo->outputMixEnvironmentalReverb = outputMixEnvironmentalReverb;

    LOGI("audio", "初始化AudioPlayer成功");

    return (long) playInfo;
}

void destroyUrlAudioPlayer(long playPtr) {
    UrlAudioPlayerInfo *playInfo = (UrlAudioPlayerInfo *) playPtr;

    if (NULL == playInfo->engineObject)return;

    if (NULL != playInfo->outputMixObject) {
        (*(playInfo->engineObject))->Destroy(playInfo->outputMixObject);
        playInfo->outputMixObject = NULL;
    }

    if (NULL != playInfo->uriPlayerObject) {
        (*(playInfo->uriPlayerObject))->Destroy(playInfo->uriPlayerObject);
        playInfo->uriPlayerObject = NULL;
    }


    if (NULL != playInfo->engineObject) {
        (*(playInfo->engineObject))->Destroy(playInfo->engineObject);
        playInfo->engineEngine = NULL;
    }

    free(playInfo);

    LOGI("audio", "销毁AudioPlayer完毕");

}

void initUrlAudioPlayer(JNIEnv *env, long playPtr, jstring url) {
    UrlAudioPlayerInfo *playInfo = (UrlAudioPlayerInfo *) playPtr;
    SLEngineItf engineEngine = playInfo->engineEngine;

    SLresult result;

    // 解析出音频数据
    const char *audioUrl = env->GetStringUTFChars(url, NULL);
    LOGI("audio", "开始初始化url %s", audioUrl);

    // 配置音源
    SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar *) audioUrl};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_uri, &format_mime};

    // 配置音频接收器
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, playInfo->outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // 创建音频播放器
    SLObjectItf uriPlayerObject;
    const SLInterfaceID ids[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &uriPlayerObject, &audioSrc, &audioSnk, 3, ids, req);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "创建播放器失败");
    }
    playInfo->uriPlayerObject = uriPlayerObject;

    // 初始化
    result = (*uriPlayerObject)->Realize(uriPlayerObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "初始化播放器失败");
    }

    SLPlayItf uriPlayerPlay;
    result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PLAY, &uriPlayerPlay);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "创建播放器控制失败");
    }
    playInfo->uriPlayerPlay = uriPlayerPlay;

    (*uriPlayerPlay)->SetCallbackEventsMask(uriPlayerPlay, SL_PLAYEVENT_HEADATEND);
    (*uriPlayerPlay)->SetCallbackEventsMask(uriPlayerPlay, SL_PLAYEVENT_HEADATMARKER);
    (*uriPlayerPlay)->SetCallbackEventsMask(uriPlayerPlay, SL_PLAYEVENT_HEADATNEWPOS);
    (*uriPlayerPlay)->SetCallbackEventsMask(uriPlayerPlay, SL_PLAYEVENT_HEADMOVING);
    (*uriPlayerPlay)->SetCallbackEventsMask(uriPlayerPlay, SL_PLAYEVENT_HEADSTALLED);

    result = (*uriPlayerPlay)->RegisterCallback(uriPlayerPlay, urlPlayCallback, playInfo);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "RegisterCallback失败");
    }

    LOGI("audio", "初始化uri播放器成功");
}

void urlAudioPlayerPlay(long playPtr) {
    UrlAudioPlayerInfo *playInfo = (UrlAudioPlayerInfo *) playPtr;
    SLPlayItf uriPlayerPlay = playInfo->uriPlayerPlay;
    SLresult result;

    result = (*uriPlayerPlay)->SetPlayState(uriPlayerPlay, SL_PLAYSTATE_PLAYING);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "播放失败");
    }

    LOGE("audio", "播放成功");
}

unsigned int urlAudioPlayerGetState(long playPtr) {
    UrlAudioPlayerInfo *playInfo = (UrlAudioPlayerInfo *) playPtr;
    SLPlayItf uriPlayerPlay = playInfo->uriPlayerPlay;
    SLresult result;

    SLuint32 state;
    result = (*uriPlayerPlay)->GetPlayState(uriPlayerPlay, &state);

    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "获取状态失败");
        return -1;
    } else {
        return state;
    }
}
