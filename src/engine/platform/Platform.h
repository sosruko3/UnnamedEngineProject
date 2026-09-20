#pragma once
#include <stdint.h>
union SDL_Event;
class LogSystem;

class Platform {
public:
    LogSystem* log = nullptr;
    bool init();
    void shutdown();
    static uint64_t getTicksNS();
    const char* GetPrefPath();
    const char* GetSummary() const; // logSystem is not initialized yet so we using this.
    bool pollEvent(SDL_Event& event);
};

