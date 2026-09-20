#pragma once
//#include <SDL3/SDL_scancode.h> included in EngineKeys.h
//#include <stdint.h> included in EngineKeys.h
#include "engine/common/EngineKeys.h"
#include <array>

union SDL_Event;

class InputSystem {
public:
  bool init(); // Empty for now. No initialization needed.
  void shutdown(); // Empty for now. No shutdown needed.
  void handleEvent(const SDL_Event& event);
  void beginFrame();
  
  bool isKeyDown(Key k) const;
  bool wasKeyPressed(Key k) const;
  bool wasKeyReleased(Key k) const;

  float getMouseX() const { return mouseX; }
  float getMouseY() const { return mouseY; }
private:
    bool initialized = false;
    constexpr static int KEY_COUNT = SDL_SCANCODE_COUNT;
    std::array<bool, KEY_COUNT> current{};
    std::array<bool, KEY_COUNT> previous{};
 
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    
};

