#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <string>

class MediaPlayer {
public:
    MediaPlayer(const std::string& file_path);
    ~MediaPlayer();

    void initialize();
    void play();
    void pause();
    void stop();
    void seek(int64_t timestamp);
    void handleEvents();
    void cleanup();

public:
    void toggleFullscreen();  // Add this method to MediaPlayer

private:
    std::string file_path;
    bool is_running;
    bool is_paused;

    class Decoder* decoder;
    class VideoRenderer* videoRenderer;
    class EventHandler* eventHandler;
};

#endif // MEDIA_PLAYER_H
