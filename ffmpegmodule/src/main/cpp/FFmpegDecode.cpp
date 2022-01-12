//
// Created by JOYY on 2022/1/11.
//
#include "IFFmpegDecode.h"

void *_decode(void *argv) {
    arg_decode *arg = (arg_decode *) argv;
    LOGI("play", "在线程中准备开始软解 %s", arg->url);

    AVFormatContext *formatContext = avformat_alloc_context();

    // 打开一个输入流
    if (avformat_open_input(&formatContext, arg->url, NULL, NULL) != 0) {
        LOGE("play", "不能打开对应文件 %s", arg->url);
        pthread_exit(NULL);
    }

    // 找到相关流信息
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        LOGE("play", "找不到相关流信息");
        pthread_exit(NULL);
    }

    // 找出视频流
    int videoStream = -1, i;
    for (i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO
            && videoStream < 0) {
            videoStream = i;
            break;
        }
    }
    if (videoStream == -1) {
        LOGE("play", "找不到视频流");
        pthread_exit(NULL);
    }

    // 获得流的描述
    AVCodecParameters *avCodecParam = formatContext->streams[videoStream]->codecpar;

    // 获得对应解码器
    const AVCodec *avCodec = avcodec_find_decoder(avCodecParam->codec_id);
/*  硬解码的写法
    switch (avCodecParam->codec_id){
        case AV_CODEC_ID_H264:
            avCodec = avcodec_find_decoder_by_name("h264_mediacodec");//硬解码264
            if (avCodec == NULL) {
                LOGE("Couldn't find Codec.\n");
                return -1;
            }
            break;
        case AV_CODEC_ID_MPEG4:
            avCodec = avcodec_find_decoder_by_name("mpeg4_mediacodec");//硬解码mpeg4
            if (avCodec == NULL) {
                LOGE("Couldn't find Codec.\n");
                return -1;
            }
            break;
        case AV_CODEC_ID_HEVC:
            avCodec = avcodec_find_decoder_by_name("hevc_mediacodec");//硬解码265
            if (avCodec == NULL) {
                LOGE("Couldn't find Codec.\n");
                return -1;
            }
            break;
        default:
            break;
    }*/

    // 在每个数据包开始时调用此回调以获取数据为它缓冲。（解码上下文）
    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);

    if (avcodec_parameters_to_context(avCodecContext, avCodecParam) != 0) {
        LOGE("play", "无法复制编解码器上下文");
        pthread_exit(NULL);
    }

    LOGI("play", "视频流帧率：%d fps\n", formatContext->streams[videoStream]->r_frame_rate.num /
                                   formatContext->streams[videoStream]->r_frame_rate.den);

    int iTotalSeconds = (int) formatContext->duration / 1000000;
    int iHour = iTotalSeconds / 3600;//小时
    int iMinute = iTotalSeconds % 3600 / 60;//分钟
    int iSecond = iTotalSeconds % 60;//秒
    LOGI("play", "持续时间：%02d:%02d:%02d\n", iHour, iMinute, iSecond);

    LOGI("play", "获取解码器SUCCESS");

    // 打开解码器
    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        LOGE("play", "无法打开编解码器。");
        pthread_exit(NULL);
    }

    ANativeWindow *nativeWindow = arg->window;

    // 获取视频宽高
    int videoWidth = avCodecContext->width;
    int videoHeight = avCodecContext->height;
    LOGI("play", "设置native window的buffer大小,可自动拉伸");
    ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight,
                                     WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer windowBuffer;

    AVFrame *frameDecode = av_frame_alloc();
    AVFrame *frameRGBA = av_frame_alloc();

    if (frameRGBA == NULL || frameDecode == NULL) {
        LOGE("play", "无法分配视频帧。");
        pthread_exit(NULL);
    }

    // 获得帧数据大小
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, avCodecContext->width, avCodecContext->height, 1);

    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(frameRGBA->data, frameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                         avCodecContext->width, avCodecContext->height, 1);

    LOGI("play", "由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换");
    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    struct SwsContext *swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                                   avCodecContext->pix_fmt,
                                                   avCodecContext->width,
                                                   avCodecContext->height, AV_PIX_FMT_RGBA,
                                                   SWS_BICUBIC, NULL, NULL, NULL);
    if (swsContext == NULL) {
        LOGE("play", "无法初始化转换上下文！");
        pthread_exit(NULL);
    }
    LOGI("play", "格式转换成功");

    LOGE("play", "开始播放");
    int ret;
    AVPacket packet;
    // 获得下一帧
    while (av_read_frame(formatContext, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream) {
            //该楨位置
            double timestamp = packet.pts * av_q2d(formatContext->streams[videoStream]->time_base);
            LOGI("play", "timestamp=%f", timestamp);
            // 解码
            ret = avcodec_send_packet(avCodecContext, &packet);
            if (ret < 0) {
                break;
            }
            // 获得解码后帧数据(如果能解出来)
            while (avcodec_receive_frame(avCodecContext, frameDecode) == 0) {//绘图
                // lock native window buffer
                ANativeWindow_lock(nativeWindow, &windowBuffer, 0);
                // 格式转换
                sws_scale(swsContext, (uint8_t const *const *) frameDecode->data,
                          frameDecode->linesize, 0, avCodecContext->height,
                          frameRGBA->data, frameRGBA->linesize);
                // 获取stride
                uint8_t *dst = (uint8_t *) windowBuffer.bits;
                int dstStride = windowBuffer.stride * 4;
                uint8_t *src = frameRGBA->data[0];
                int srcStride = frameRGBA->linesize[0];

                // 由于window的stride和帧的stride不同,因此需要逐行复制
                int h;
                for (h = 0; h < videoHeight; h++) {
                    memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
                }
                ANativeWindow_unlockAndPost(nativeWindow);
            }
        }
        // 清掉这一个的信息
        av_packet_unref(&packet);
    }
    LOGE("play", "播放完成");

    // 回收资源
    av_free(frameDecode);
    av_free(frameRGBA);
    av_free(buffer);
    avcodec_close(avCodecContext);
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
    ANativeWindow_release(nativeWindow);
    free(arg);

    pthread_exit(NULL);
}

void play(JNIEnv *env, jclass clazz, jstring uri, jobject surface) {

    const char *file_name = env->GetStringUTFChars(uri, NULL);

    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (!nativeWindow) {
        LOGE("play", "ANativeWindow 获取错误");
    }

    pthread_t threadPrt;

    arg_decode *arg = new arg_decode();
    arg->url = file_name;
    arg->window = nativeWindow;

    pthread_create(&threadPrt, NULL, _decode, arg);
}
