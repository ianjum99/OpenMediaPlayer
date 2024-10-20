#include "VideoRenderer.h"
#include <iostream>

VideoRenderer::VideoRenderer() 
    : window(nullptr), renderer(nullptr), texture(nullptr) {}

VideoRenderer::~VideoRenderer() {
    cleanup();
}

void VideoRenderer::initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow("Media Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        std::cerr << "Texture could not be created! SDL Error: " << SDL_GetError() << std::endl;
    }
}

void VideoRenderer::renderFrame(AVFrame* frame) {
    SDL_UpdateYUVTexture(texture, nullptr, frame->data[0], frame->linesize[0],
                         frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void VideoRenderer::toggleFullscreen() {
    Uint32 fullscreen_flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
    bool is_fullscreen = SDL_GetWindowFlags(window) & fullscreen_flag;
    SDL_SetWindowFullscreen(window, is_fullscreen ? 0 : fullscreen_flag);
}

void VideoRenderer::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}
