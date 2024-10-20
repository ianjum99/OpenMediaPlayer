#include "MediaPlayer.h"
#include <iostream>
#include <SDL2/SDL.h>

MediaPlayer::MediaPlayer(const std::string& file_path)
    : decoder(nullptr), videoRenderer(nullptr) {
    decoder = new Decoder(file_path);
}

MediaPlayer::~MediaPlayer() {
    cleanup();
}

bool MediaPlayer::initialize() {
    if (!decoder->initialize()) {
        std::cerr << "Failed to initialize the decoder." << std::endl;
        return false;
    }

    if (decoder->video_stream_index != -1) {
        videoRenderer = new VideoRenderer();
        videoRenderer->initialize(decoder->getWidth(), decoder->getHeight());
    }

    return true;  // Initialization succeeded
}

void MediaPlayer::play() {
    AVFrame* frame = av_frame_alloc();

    // Handle audio-only files
    if (decoder->audio_stream_index != -1 && decoder->video_stream_index == -1) {
        std::cout << "Playing audio-only file." << std::endl;

        // Initialize SDL for audio playback
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
            return;
        }

        AVCodecContext* audioCodecCtx = decoder->getAudioCodecCtx();

        // Set the audio format using SDL
        SDL_AudioSpec spec;
        spec.freq = audioCodecCtx->sample_rate;
        spec.format = AUDIO_S16SYS;  // 16-bit signed audio format
        spec.channels = audioCodecCtx->channels;
        spec.silence = 0;
        spec.samples = 1024;
        spec.callback = nullptr;

        if (SDL_OpenAudio(&spec, nullptr) < 0) {
            std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
            return;
        }

        // Start playing audio
        SDL_PauseAudio(0);

        AVPacket packet;
        av_init_packet(&packet);

        // Read and decode audio frames
        bool audio_finished = false;
        while (av_read_frame(decoder->getFormatCtx(), &packet) >= 0) {
            if (packet.stream_index == decoder->audio_stream_index) {
                if (avcodec_send_packet(audioCodecCtx, &packet) >= 0) {
                    while (avcodec_receive_frame(audioCodecCtx, frame) == 0) {
                        // Queue the audio frame data for playback
                        SDL_QueueAudio(1, frame->data[0], frame->linesize[0]);
                    }
                }
            }
            av_packet_unref(&packet);
        }

        // Wait for audio to finish playing
        while (SDL_GetQueuedAudioSize(1) > 0) {
            SDL_Delay(100);
        }

        // Cleanup SDL resources
        SDL_CloseAudio();
        SDL_Quit();

    } else if (decoder->video_stream_index != -1) {
        // Video playback logic
        std::cout << "Playing video file." << std::endl;
        while ((frame = decoder->decodeVideoFrame())) {
            videoRenderer->renderFrame(frame);
        }
    } else {
        std::cerr << "No audio or video stream found." << std::endl;
    }

    av_frame_free(&frame);
}

void MediaPlayer::pause() {
    std::cout << "Media paused" << std::endl;
}

void MediaPlayer::toggleFullscreen() {
    if (videoRenderer) {
        videoRenderer->toggleFullscreen();
    }
}

void MediaPlayer::seek(long timestamp) {
    decoder->seek(timestamp);
}

void MediaPlayer::cleanup() {
    if (decoder) {
        decoder->cleanup();
        delete decoder;
        decoder = nullptr;
    }

    if (videoRenderer) {
        videoRenderer->cleanup();
        delete videoRenderer;
        videoRenderer = nullptr;
    }
}
