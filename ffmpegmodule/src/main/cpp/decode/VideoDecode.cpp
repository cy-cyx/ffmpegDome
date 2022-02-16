//
// Created by 小新 on 2022/2/13.
//
#include "decode/IVideoDecode.h"

void fm_VideoDecode::initDecode() {

}

void fm_VideoDecode::execute(const char *fileName) {
    url = fileName;
    pthread_create(&threadPrt, NULL, fm_VideoDecode::decodeTask, this);
}

void *fm_VideoDecode::decodeTask(void *decode) {

    fm_VideoDecode *decoder = (fm_VideoDecode *) decode;

    do {
        LOGI("decode", "开始解码视频 %s", decoder->url);

        AVFormatContext *formatContext = avformat_alloc_context();

        // 打开一个输入流
        if (avformat_open_input(&formatContext, decoder->url, NULL, NULL) != 0) {
            LOGE("decode", "不能打开对应文件 %s", decoder->url);
            break;
        }

        // 找到相关流信息
        if (avformat_find_stream_info(formatContext, NULL) < 0) {
            LOGE("decode", "找不到相关流信息");
            break;
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
            LOGE("decode", "找不到视频流");
            break;
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
            LOGE("decode", "无法复制编解码器上下文");
            break;
        }

        // 设置一下帧
        decoder->videoRender->setBuffersGeometry(avCodecContext->width, avCodecContext->height,
                                                 WINDOW_FORMAT_RGBA_8888);

        LOGI("decode", "视频流帧率：%d fps\n", formatContext->streams[videoStream]->r_frame_rate.num /
                                         formatContext->streams[videoStream]->r_frame_rate.den);

        // 打开解码器
        if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
            LOGE("decode", "无法打开编解码器。");
            break;
        }

        AVFrame *frameDecode = av_frame_alloc();

        if (frameDecode == NULL) {
            LOGE("decode", "无法分配视频帧。");
            break;
        }

        // 获得帧数据大小
        int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, avCodecContext->width, avCodecContext->height, 1);

        LOGI("decode", "由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换");
        // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
        struct SwsContext *swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                                       avCodecContext->pix_fmt,
                                                       avCodecContext->width,
                                                       avCodecContext->height, AV_PIX_FMT_RGBA,
                                                       SWS_BICUBIC, NULL, NULL, NULL);
        if (swsContext == NULL) {
            LOGE("decode", "无法初始化转换上下文！");
            break;
        }

        LOGI("decode", "初始化格式转换成功");

        LOGE("decode", "开始解码");

        uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

        int ret;
        AVPacket packet;
        // 获得下一帧
        while (av_read_frame(formatContext, &packet) >= 0) {
            // Is this a packet from the video stream?
            if (packet.stream_index == videoStream) {
                //该楨位置
                double timestamp = packet.pts * av_q2d(formatContext->streams[videoStream]->time_base);
                LOGI("decode", "timestamp=%f", timestamp);
                // 解码
                ret = avcodec_send_packet(avCodecContext, &packet);
                if (ret < 0) {
                    break;
                }
                // 获得解码后帧数据(如果能解出来)
                while (avcodec_receive_frame(avCodecContext, frameDecode) == 0) {//绘图

                    // 获得转码的缓冲区
                    AVFrame *frameRGBA = av_frame_alloc();

                    if (frameRGBA == NULL) {
                        LOGE("decode", "无法分配转码视频帧");
                        continue;
                    }

                    av_image_fill_arrays(frameRGBA->data, frameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                                         avCodecContext->width, avCodecContext->height, 1);

                    // 格式转换
                    sws_scale(swsContext, (uint8_t const *const *) frameDecode->data,
                              frameDecode->linesize, 0, avCodecContext->height,
                              frameRGBA->data, frameRGBA->linesize);

                    LOGE("decode", "塞入绘制队列");

                    decoder->videoRender->pullFrame(frameRGBA, timestamp, avCodecContext->width,
                                                    avCodecContext->height);
                }
            }
            // 清掉这一个的信息
            av_packet_unref(&packet);
        }
        LOGE("decode", "播放完成");

    } while (false);

    decoder->releaseDecode();

    pthread_exit(NULL);
}

void fm_VideoDecode::releaseDecode() {

}
