#include "InputSystem.h"
#include <SDL3/SDL_events.h>
#include <cstddef>

bool InputSystem::init() {
    if (initialized) return true;
    
    initialized = true;
    return true;
}
void InputSystem::shutdown() {
    if (!initialized) return;
    initialized = false;
}
void InputSystem::beginFrame() { // call this function before handling events
    previous = current;
    
}

void InputSystem::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            const std::size_t scancode = static_cast<std::size_t>(event.key.scancode);
            if (!event.key.repeat && scancode < KEY_COUNT)
                current[scancode] = true;
            break;
        }

        case SDL_EVENT_KEY_UP: {
            const std::size_t scancode = static_cast<std::size_t>(event.key.scancode);
            if (scancode < KEY_COUNT)
                current[scancode] = false;
            break;
        }

        case SDL_EVENT_WINDOW_FOCUS_LOST:
            current.fill(false);
            previous.fill(false);
            break;

        case SDL_EVENT_MOUSE_MOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;

        default:
            break;
        }
    
}

bool InputSystem::isKeyDown(Key k) const {
    const std::size_t index = static_cast<std::size_t>(k);
    if (index >= KEY_COUNT) return false;
    
    return current[index];
}

bool InputSystem::wasKeyPressed(Key k) const {
    const std::size_t index = static_cast<std::size_t>(k);
    if (index >= KEY_COUNT) return false;
    
    return (current[index]) &&
    (!previous[index]);
}

bool InputSystem::wasKeyReleased(Key k) const {
    const std::size_t index = static_cast<std::size_t>(k);
    if (index >= KEY_COUNT) return false;
    
    return (!current[index]) &&
    (previous[index]);
}

