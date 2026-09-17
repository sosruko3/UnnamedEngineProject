#pragma once

#include "engine/platform/Platform.h"
#include "engine/platform/WindowSystem.h"
#include "engine/platform/InputSystem.h"
#include "engine/platform/TimeSystem.h"
#include "engine/memory/Arena.h"

struct EntityRegistry;
struct CommandBus;
union SDL_Event;

class Engine {
public:
    bool init(Arena* masterArena);
    void run();
    void shutdown();
    void pollEvents();
    
    // temp
    void EnginePhase0_PlatformSync();
    void EnginePhase1_InputAndLogic();
    void EnginePhase2_Simulation();
    void EnginePhase3_RenderState();
    void EnginePhase4_Cleanup();
    
private:
    const char* title = "";
    const char* configFileName = "";
    Platform platform;
    WindowSystem windowSystem;
    InputSystem inputSystem;
    TimeContext time;
    EntityRegistry *reg;
    CommandBus *bus;
    
    Arena entityArena;
    Arena physicsArena;
    Arena busArena;
    Arena audioArena;
    Arena frameArena;
    
    bool running = false;
};
