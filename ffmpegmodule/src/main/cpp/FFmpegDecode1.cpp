//
// Created by JOYY on 2022/1/20.
//
#include "IFFmpegDecode.h"

AVSampleFormat getAVSampleFormatByFormat(int format) {
    switch (format) {
        case 0:
            return AV_SAMPLE_FMT_U8;
        case 1:
            return AV_SAMPLE_FMT_S16;
        case 2:
            return AV_SAMPLE_FMT_S32;
        case 3:
            return AV_SAMPLE_FMT_FLT;
        case 4:
            return AV_SAMPLE_FMT_DBL;
        case 5:
            return AV_SAMPLE_FMT_U8P;
        case 6:
            return AV_SAMPLE_FMT_S16P;
        case 7:
            return AV_SAMPLE_FMT_S32P;
        case 8:
            return AV_SAMPLE_FMT_FLTP;
        case 9:
            return AV_SAMPLE_FMT_DBLP;
        case 10:
            return AV_SAMPLE_FMT_S64;
        case 11:
            return AV_SAMPLE_FMT_S64P;
        case 12:
            return AV_SAMPLE_FMT_NB;
    }
}

void *_decodeAudio(void *argv) {

    AudioDecodeInfo *info = (AudioDecodeInfo *) argv;

    LOGI("decode", "开始解码音频 %s", info->url);

    AVFormatContext *formatContext = avformat_alloc_context();

    // 打开一个输入流
    if (avformat_open_input(&formatContext, info->url, NULL, NULL) != 0) {
        LOGE("play", "不能打开对应文件 %s", info->url);
        pthread_exit(NULL);
    }

    // 找到相关流信息
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        LOGE("play", "找不到相关流信息");
        pthread_exit(NULL);
    }

    // 找出视频流
    int audioStream = -1, i;
    for (i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO
            && audioStream < 0) {
            audioStream = i;
            break;
        }
    }
    if (audioStream == -1) {
        LOGE("play", "找不到视频流");
        pthread_exit(NULL);
    }

    // 获得流的描述
    AVCodecParameters *avCodecParam = formatContext->streams[audioStream]->codecpar;

    // 采样频率
    int sample_rate = avCodecParam->sample_rate;
    LOGI("decode", "采样频率 %d", sample_rate);

    // 采样位数
    int format = avCodecParam->format;
    LOGI("decode", "采样的位数 %d", format);

    AVSampleFormat sampleFormat = getAVSampleFormatByFormat(format);
    (void) sampleFormat;

    // 通道数
    int channels = avCodecParam->channels;
    uint64_t channel_layout = avCodecParam->channel_layout;

    LOGI("decode", "通道数 %d 通道掩码 %lu", channels, channel_layout);

    // 获得对应解码器
    const AVCodec *avCodec = avcodec_find_decoder(avCodecParam->codec_id);
    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);

    if (avcodec_parameters_to_context(avCodecContext, avCodecParam) != 0) {
        LOGE("decode", "无法复制编解码器上下文");
        pthread_exit(NULL);
    }

    // 打开解码器
    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        LOGE("decode", "无法打开编解码器。");
        pthread_exit(NULL);
    }

    SwrContext *swrContext = swr_alloc();
    if (!swrContext) {
        LOGE("decode", "新建编解码器失败。");
        pthread_exit(NULL);
    }
    av_opt_set_int(swrContext, "in_channel_layout", channel_layout, 0);
    av_opt_set_int(swrContext, "in_sample_rate", sample_rate, 0);
    av_opt_set_sample_fmt(swrContext, "in_sample_fmt", sampleFormat, 0);

    av_opt_set_int(swrContext, "out_channel_layout", outSwrChannelLayout, 0);
    av_opt_set_int(swrContext, "out_sample_rate", outSwrSampleRate, 0);
    av_opt_set_sample_fmt(swrContext, "out_sample_fmt", outSwrSampleFormat, 0);
    int code = swr_init(swrContext);
    if (code != 0) {
        LOGE("decode", "初始化编解码器失败。");
        pthread_exit(NULL);
    }

    int outSwrNbSamples = (int) av_rescale_rnd(ACC_NB_SAMPLES, outSwrSampleRate, avCodecContext->sample_rate,
                                               AV_ROUND_UP);
    LOGI("decode", "输出的样本大小 %d", outSwrNbSamples);
    int outSwrFrameDataSize = av_samples_get_buffer_size(NULL, outSwrChannelLayout, outSwrNbSamples, outSwrSampleFormat,
                                                         1);
    LOGI("decode", "输出的数据大小 %d", outSwrFrameDataSize);
    uint8_t *outSwrAudioOutBuffer = (uint8_t *) malloc(outSwrFrameDataSize);
//  备注由于带P和不带P  的存储方式是不同 故用错时会有空指针的崩溃
//    uint8_t *outSwrAudioOutBuffer[2];
//    outSwrAudioOutBuffer[0] = (uint8_t *) malloc(outSwrFrameDataSize);
//    outSwrAudioOutBuffer[1] = (uint8_t *) malloc(outSwrFrameDataSize);

    LOGE("decode", "开始解码");

    int ret;
    AVPacket packet;
    AVFrame *frameDecode = av_frame_alloc();

    // 获得下一帧
    while (av_read_frame(formatContext, &packet) == 0) {
        if (packet.stream_index == audioStream) {

            double timestamp = packet.pts * av_q2d(formatContext->streams[audioStream]->time_base);
            LOGI("decode", "timestamp=%f", timestamp);

            ret = avcodec_send_packet(avCodecContext, &packet);
            LOGE("decode", "塞入解码器 %d", ret);

            while (avcodec_receive_frame(avCodecContext, frameDecode) == 0) {
                LOGE("decode", "从解码器中，取出一帧的音频数据 样本数 %d", frameDecode->nb_samples);
                int result = swr_convert(swrContext, &outSwrAudioOutBuffer, outSwrFrameDataSize / outSwrChannel,
                                         (const uint8_t **) frameDecode->data, frameDecode->nb_samples);
                if (result > 0) {
                    LOGE("decode", "重采样成功 重采样样本数 %d", result);
                }
            }
        }
        // 清掉这一个帧的数据了
        av_packet_unref(&packet);
    }

    LOGE("decode", "解码结束");

    av_frame_free(&frameDecode);
    avcodec_close(avCodecContext);
    avformat_free_context(formatContext);

    pthread_exit(NULL);
}

void playVideoOfAudio(JNIEnv *env, jclass clazz, jstring uri) {
    pthread_t threadPrt;

    const char *file_name = env->GetStringUTFChars(uri, NULL);
    LOGI("decode", "播放视频中的音频 %s", file_name);

    // 初始化解码信息
    BlockQueue *queue = new BlockQueue();
    AudioDecodeInfo *info = new AudioDecodeInfo();
    info->url = file_name;
    info->queue = queue;

    // 开一条线程解码，一条播放
    pthread_create(&threadPrt, NULL, _decodeAudio, info);
}

