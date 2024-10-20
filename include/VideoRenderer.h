#ifndef VIDEO_RENDERER_H
#define VIDEO_RENDERER_H

#include <SDL.h>

class VideoRenderer {
public:
    VideoRenderer();
    ~VideoRenderer();

    void initialize(int width, int height);
    void renderFrame(AVFrame* frame);
    void toggleFullscreen();
    void cleanup();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif // VIDEO_RENDERER_H
