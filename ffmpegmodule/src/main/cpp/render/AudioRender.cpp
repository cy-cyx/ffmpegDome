//
// Created by JOYY on 2022/1/29.
//
#include "../../include/render/IAudioRender.h"


void fm_AudioRender::initRender() {
    queueSize = 2;
    blockQueue = new BlockQueue;

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

    // 获取引擎接口，这是创建其他对象所必需的
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "创建引擎接口失败");
    }

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

    // 设置环境混响
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        SLEnvironmentalReverbSettings reverbSettings =
                SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        if (SL_RESULT_SUCCESS != result) {
            LOGE("audio", "设置环境混音失败");
        }
    } else {
        LOGE("audio", "获得环境混音失败");
    }

    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,//format type
            (SLuint32) 2,// 重采样后声道2
            SL_SAMPLINGRATE_44_1,// 重采样后44100hz
            SL_PCMSAMPLEFORMAT_FIXED_16,// 重采样后每个样本16位
            SL_PCMSAMPLEFORMAT_FIXED_16,// container size
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,// channel mask
            SL_BYTEORDER_LITTLEENDIAN // endianness
    };
    SLDataSource slDataSource = {&android_queue, &pcm};

    // 配置音频接收器
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    const SLInterfaceID ids2[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req2[1] = {SL_BOOLEAN_TRUE};

    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &slDataSource, &audioSnk, 1, ids2, req2);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "创建播放器失败");
    }

    result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "初始化播放器失败");
    }

    result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &uriPlayerPlay);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "创建播放器控制失败");
    }

    result = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &bufferQueue);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "创建播放器播放队列");
    }

    result = (*bufferQueue)->RegisterCallback(bufferQueue, fm_AudioRender::queueCallback, this);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "注册队列回调失败");
    }

    result = (*uriPlayerPlay)->SetPlayState(uriPlayerPlay, SL_PLAYSTATE_PLAYING);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("audio", "播放失败");
    }
}

void fm_AudioRender::queueCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context) {
    LOGE("audio", "队列回调");
    fm_AudioRender *render = (fm_AudioRender *) context;

    // 判断时间戳

    BlockQueue *queue = render->blockQueue;
    fm_AudioFrame *frame = (fm_AudioFrame *) queue->poll();
    LOGE("audio", "取出缓存队列");

    SLAndroidSimpleBufferQueueItf bufferQueueItf = render->bufferQueue;
    SLresult queueResult = (*bufferQueueItf)->Enqueue(bufferQueueItf, frame->frameData, frame->size);
    if (SL_RESULT_SUCCESS == queueResult) {
        LOGE("audio", "插入队列成功呢(回调里)");
    }
}

void fm_AudioRender::pullFrame(uint8_t *frameData, int size, double timestamp) {

    // 前几帧直接塞入，后几帧塞入队列
    if (queueSize > 0) {
        SLresult queueResult = (*bufferQueue)->Enqueue(bufferQueue, frameData, size);
        if (SL_RESULT_SUCCESS == queueResult) {
            LOGE("audio", "插入播放队列成功(直接)");
        }
        queueSize--;
    } else {
        fm_AudioFrame *frame = new fm_AudioFrame();
        frame->frameData = frameData;
        frame->size = size;
        frame->timestamp = timestamp;
        blockQueue->pull(frame);
        LOGE("audio", "插入队列成功(直接)");
    }
}

void fm_AudioRender::releaseRender() {

}