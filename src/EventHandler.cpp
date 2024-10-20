#include "EventHandler.h"
#include "MediaPlayer.h"
#include <SDL.h>

EventHandler::EventHandler() : is_paused(false) {}

EventHandler::~EventHandler() {}

bool EventHandler::handleEvents(MediaPlayer& mediaPlayer) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_SPACE:
                    mediaPlayer.pause();
                    break;
                case SDLK_f:
                    mediaPlayer.toggleFullscreen();
                    break;
                case SDLK_RIGHT:
                    mediaPlayer.seek(10 * AV_TIME_BASE); // Seek 10 seconds forward
                    break;
                case SDLK_LEFT:
                    mediaPlayer.seek(-10 * AV_TIME_BASE); // Seek 10 seconds backward
                    break;
                case SDLK_q:
                    return false;
            }
        }
    }
    return true;
}

void EventHandler::togglePause() {
    is_paused = !is_paused;
}
