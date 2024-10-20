#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <string>
#include "Decoder.h"
#include "VideoRenderer.h"

class MediaPlayer {
public:
    MediaPlayer(const std::string& file_path);
    ~MediaPlayer();

    bool initialize();  // Changed from void to bool
    void play();
    void pause();
    void toggleFullscreen();
    void seek(long timestamp);
    void cleanup();

private:
    Decoder* decoder;
    VideoRenderer* videoRenderer;
};

#endif // MEDIA_PLAYER_H
