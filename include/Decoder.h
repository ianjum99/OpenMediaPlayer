#ifndef DECODER_H
#define DECODER_H

#include <string>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
}

class Decoder {
public:
    Decoder(const std::string& file_path);
    ~Decoder();

    bool initialize();
    AVFrame* decodeVideoFrame();
    void cleanup();
    
    void seek(int64_t timestamp);

    int getWidth() const { return videoCodecCtx->width; }
    int getHeight() const { return videoCodecCtx->height; }

    // Add these getter methods to access private members
    AVCodecContext* getAudioCodecCtx() { return audioCodecCtx; }
    AVFormatContext* getFormatCtx() { return pFormatCtx; }

    int video_stream_index;
    int audio_stream_index;

private:
    AVFormatContext* pFormatCtx;
    AVCodecContext* videoCodecCtx;
    AVCodecContext* audioCodecCtx;
    SwsContext* swsCtx;
    
    AVFrame* frame;
    AVPacket* packet;
};

#endif // DECODER_H
