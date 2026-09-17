#include "Platform.h"
#include "engine/core/LogSystem.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_stdinc.h>
#include <assert.h>


static bool s_platformInitialized = false;
static char* s_allocatedPrefPath = nullptr;
static const char* s_prefPath = "";

bool Platform::init() {
    if (s_platformInitialized) return true;
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        Log(LogLevel::Error, "Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }
    s_platformInitialized = true;

    // Getting pref path
    s_allocatedPrefPath = SDL_GetPrefPath("myOrg", "myApp");
    if (!s_allocatedPrefPath) {
        Log(LogLevel::Error, "Failed to get pref path: {}", SDL_GetError());
    } else {
        s_prefPath = s_allocatedPrefPath;
    }
    return true;

}

void Platform::shutdown() {
    if (!s_platformInitialized) return;

    if (s_allocatedPrefPath) {
        SDL_free(s_allocatedPrefPath);
        s_allocatedPrefPath = nullptr;
        s_prefPath = "";
    }
    
    SDL_Quit();
    s_platformInitialized = false;
}

uint64_t Platform::getTicksNS() {
    return SDL_GetTicksNS();
}

const char* Platform::GetPrefPath() {
    return s_prefPath;
}

bool Platform::pollEvent(SDL_Event& event) {
    return SDL_PollEvent(&event);
}
