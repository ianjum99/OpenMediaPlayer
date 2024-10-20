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
    void seek(int64_t timestamp);
    int getWidth() const;
    int getHeight() const;
    void cleanup();

private:
    AVFormatContext* pFormatCtx;
    AVCodecContext* videoCodecCtx;
    AVCodecContext* audioCodecCtx;
    SwsContext* swsCtx;
    int video_stream_index;
    int audio_stream_index;
};

#endif // DECODER_H
