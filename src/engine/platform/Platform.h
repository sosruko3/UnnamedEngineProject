#pragma once
#include <stdint.h>
union SDL_Event;

class Platform {
public:
    bool init();
    void shutdown();
    static uint64_t getTicksNS();
    const char* GetPrefPath();
    bool pollEvent(SDL_Event& event);
};

