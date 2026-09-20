#include "WindowSystem.h"
#include "engine/core/LogSystem.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_error.h>


bool WindowSystem::init() {
    if (initialized)
        return true;
    
    window = SDL_CreateWindow("Title1", 1280 , 720, SDL_WINDOW_RESIZABLE);
    if (!window) {
        log->Error("Failed to create window: {}", SDL_GetError());
        return false;
    }
    if (!SDL_GetWindowSize(window, &width, &height)) {
        // might also need Pixel version of this window size, check this again later on.
        log->Error("SDL_GetWindowSize failed: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        window = nullptr;
        return false;
    }
    initialized = true;
    return true;
}
void WindowSystem::shutdown() {
    if (!initialized)
        return;
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    initialized = false;
}
void WindowSystem::handleEvent(const SDL_Event& event) {
    // TODO: All window events
    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            width = event.window.data1;
            height = event.window.data2;
            break;
        case SDL_EVENT_QUIT: // same as SDL_EVENT_WINDOW_CLOSE_REQUESTED
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            wantsClose = true;
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            focused = true;
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            focused = false;
            break;

        default:
            break;
    }
}
