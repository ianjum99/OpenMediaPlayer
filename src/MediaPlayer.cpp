#include "MediaPlayer.h"
#include "Decoder.h"
#include "VideoRenderer.h"
#include "EventHandler.h"
#include <iostream>

MediaPlayer::MediaPlayer(const std::string& file_path) 
    : file_path(file_path), is_running(false), is_paused(false),
      decoder(nullptr), videoRenderer(nullptr), eventHandler(nullptr) {}

MediaPlayer::~MediaPlayer() {
    cleanup();
}

void MediaPlayer::initialize() {
    // Initialize decoder
    decoder = new Decoder(file_path);
    if (!decoder->initialize()) {
        std::cerr << "Failed to initialize the decoder." << std::endl;
        return;
    }

    // Initialize video renderer with video dimensions from decoder
    videoRenderer = new VideoRenderer();
    videoRenderer->initialize(decoder->getWidth(), decoder->getHeight());

    // Initialize event handler
    eventHandler = new EventHandler();
}

void MediaPlayer::play() {
    is_running = true;
    while (is_running) {
        handleEvents();
        if (!is_paused) {
            AVFrame* frame = decoder->decodeVideoFrame();
            if (frame != nullptr) {
                videoRenderer->renderFrame(frame);
            }
        }
    }
}

void MediaPlayer::pause() {
    is_paused = !is_paused;
}

void MediaPlayer::stop() {
    is_running = false;
}

void MediaPlayer::seek(int64_t timestamp) {
    decoder->seek(timestamp);
}

void MediaPlayer::handleEvents() {
    if (!eventHandler->handleEvents(*this)) {
        stop();
    }
}

void MediaPlayer::cleanup() {
    if (decoder) {
        decoder->cleanup();
        delete decoder;
    }
    if (videoRenderer) {
        videoRenderer->cleanup();
        delete videoRenderer;
    }
    if (eventHandler) {
        delete eventHandler;
    }
}

void MediaPlayer::toggleFullscreen() {
    if (videoRenderer) {
        videoRenderer->toggleFullscreen();  // Delegate to VideoRenderer
    }
}
