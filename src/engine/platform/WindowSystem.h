#pragma once
struct SDL_Window;
union SDL_Event;
class LogSystem;

class WindowSystem {
public:
    LogSystem* log = nullptr;
    bool init(); // SDL_CreateWindow
    void shutdown(); // SDL_DestroyWindow
    void handleEvent(const SDL_Event& event); // SDL_PollEvent
    SDL_Window* getWindow() const { return window; }// this is for renderer, might need getNativeHandle() too.
    int  getWidth()  const { return width; } // SDL_GetWindowSize
    int  getHeight() const { return height; } // SDL_GetWindowSize
    bool shouldClose() const { return wantsClose; }
    bool hasFocus() const { return focused; }

private:
    SDL_Window* window = nullptr;
    int width = 0;
    int height = 0;
    bool initialized = false;
    bool wantsClose = false;
    bool focused = true;
};