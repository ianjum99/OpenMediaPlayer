#include "Decoder.h"
#include <iostream>
extern "C" {
    #include <libavutil/log.h>
    #include <libavutil/error.h>  // For av_strerror
}

Decoder::Decoder(const std::string& file_path)
    : video_stream_index(-1), audio_stream_index(-1),
      pFormatCtx(nullptr), videoCodecCtx(nullptr), audioCodecCtx(nullptr),
      swsCtx(nullptr), frame(nullptr), packet(nullptr) {

    // Set FFmpeg log level to debug
    av_log_set_level(AV_LOG_DEBUG);

    av_log(nullptr, AV_LOG_DEBUG, "Trying to open file: %s\n", file_path.c_str());

    // Open input file and retrieve format context
    int ret = avformat_open_input(&pFormatCtx, file_path.c_str(), nullptr, nullptr);
    if (ret != 0) {
        char error_buf[256];
        av_strerror(ret, error_buf, sizeof(error_buf));  // Get error string
        av_log(nullptr, AV_LOG_ERROR, "Could not open file: %s (Error: %s)\n", file_path.c_str(), error_buf);
        std::cerr << "Could not open file: " << file_path << " (Error: " << error_buf << ")" << std::endl;
        return;  // Return early if the file cannot be opened
    }

    av_log(nullptr, AV_LOG_DEBUG, "Successfully opened file: %s\n", file_path.c_str());
}

// Destructor
Decoder::~Decoder() {
    cleanup();
}

// Initialize method with error handling
bool Decoder::initialize() {
    if (!pFormatCtx) {
        std::cerr << "Format context not initialized. File might not be open." << std::endl;
        return false;
    }

    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        std::cerr << "Could not find stream information." << std::endl;
        return false;
    }

    for (unsigned int i = 0; i < pFormatCtx->nb_streams; ++i) {
        AVCodecParameters* codecParams = pFormatCtx->streams[i]->codecpar;
        const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);

        if (codecParams->codec_type == AVMEDIA_TYPE_VIDEO && video_stream_index < 0) {
            video_stream_index = i;
            videoCodecCtx = avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(videoCodecCtx, codecParams);
            avcodec_open2(videoCodecCtx, codec, nullptr);
        }

        if (codecParams->codec_type == AVMEDIA_TYPE_AUDIO && audio_stream_index < 0) {
            audio_stream_index = i;
            audioCodecCtx = avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(audioCodecCtx, codecParams);
            avcodec_open2(audioCodecCtx, codec, nullptr);
        }
    }

    if (video_stream_index == -1 && audio_stream_index == -1) {
        std::cerr << "Could not find audio or video stream." << std::endl;
        return false;
    }

    if (video_stream_index != -1) {
        swsCtx = sws_getContext(videoCodecCtx->width, videoCodecCtx->height,
                                videoCodecCtx->pix_fmt, videoCodecCtx->width, videoCodecCtx->height,
                                AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);
    }

    frame = av_frame_alloc();
    packet = av_packet_alloc();

    return true;
}

// Decode video frame method
AVFrame* Decoder::decodeVideoFrame() {
    if (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            avcodec_send_packet(videoCodecCtx, packet);
            if (avcodec_receive_frame(videoCodecCtx, frame) == 0) {
                av_packet_unref(packet);
                return frame;
            }
        }
        av_packet_unref(packet);
    }
    return nullptr;
}

// Seek method with logging
void Decoder::seek(int64_t timestamp) {
    av_log(nullptr, AV_LOG_DEBUG, "Seeking to timestamp: %ld\n", timestamp);

    if (av_seek_frame(pFormatCtx, video_stream_index, timestamp, AVSEEK_FLAG_BACKWARD) < 0) {
        std::cerr << "Error seeking to timestamp: " << timestamp << std::endl;
    }
}

// Cleanup method
void Decoder::cleanup() {
    if (videoCodecCtx) {
        avcodec_free_context(&videoCodecCtx);
        videoCodecCtx = nullptr;
    }

    if (audioCodecCtx) {
        avcodec_free_context(&audioCodecCtx);
        audioCodecCtx = nullptr;
    }

    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx);
        pFormatCtx = nullptr;
    }

    if (swsCtx) {
        sws_freeContext(swsCtx);
        swsCtx = nullptr;
    }

    if (frame) {
        av_frame_free(&frame);
        frame = nullptr;
    }

    if (packet) {
        av_packet_free(&packet);
        packet = nullptr;
    }
}
