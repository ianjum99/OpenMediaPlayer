#include "Decoder.h"
#include <iostream>

Decoder::Decoder(const std::string& file_path) 
    : pFormatCtx(nullptr), videoCodecCtx(nullptr), audioCodecCtx(nullptr),
      swsCtx(nullptr), video_stream_index(-1), audio_stream_index(-1) {
    // av_register_all();  // No longer needed in newer versions of FFmpeg
    if (avformat_open_input(&pFormatCtx, file_path.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open file: " << file_path << std::endl;
    }
}

Decoder::~Decoder() {
    cleanup();
}

bool Decoder::initialize() {
    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        std::cerr << "Could not find stream information." << std::endl;
        return false;
    }

    for (unsigned int i = 0; i < pFormatCtx->nb_streams; ++i) {
        AVCodecParameters* codecParams = pFormatCtx->streams[i]->codecpar;
        const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);  // Use const AVCodec*
        if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO && video_stream_index < 0) {
            video_stream_index = i;
            videoCodecCtx = avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(videoCodecCtx, codecParams);
            avcodec_open2(videoCodecCtx, codec, nullptr);
        }
    }

    if (video_stream_index == -1) {
        std::cerr << "Could not find video stream." << std::endl;
        return false;
    }

    swsCtx = sws_getContext(videoCodecCtx->width, videoCodecCtx->height,
                            videoCodecCtx->pix_fmt, videoCodecCtx->width, videoCodecCtx->height,
                            AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);

    return true;
}

AVFrame* Decoder::decodeVideoFrame() {
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    if (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            avcodec_send_packet(videoCodecCtx, packet);
            if (avcodec_receive_frame(videoCodecCtx, frame) == 0) {
                av_packet_free(&packet);
                return frame;
            }
        }
        av_packet_free(&packet);
    }

    av_frame_free(&frame);
    return nullptr;
}

void Decoder::seek(int64_t timestamp) {
    av_seek_frame(pFormatCtx, video_stream_index, timestamp, AVSEEK_FLAG_BACKWARD);
}

int Decoder::getWidth() const {
    return videoCodecCtx->width;
}

int Decoder::getHeight() const {
    return videoCodecCtx->height;
}

void Decoder::cleanup() {
    if (videoCodecCtx) {
        avcodec_free_context(&videoCodecCtx);
    }
    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx);
    }
    if (swsCtx) {
        sws_freeContext(swsCtx);
    }
}
