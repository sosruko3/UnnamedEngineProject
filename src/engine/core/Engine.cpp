#include "Engine.h"
#include "CommandBus.h"
#include "LogSystem.h"
#include "engine/core/SystemPackets.h"
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/Registry.h"
#include "engine/scene/SceneManager.h"
#include "engine/systems/audio/AudioSystem.h"
#include "engine/systems/animation/AnimationSystem.h"
#include <SDL3/SDL_events.h>
#include <stdlib.h>
#include <assert.h>


// ENGINE PHASES
void Engine::EnginePhase0_PlatformSync() {
  timeSystem.update();
  
}

void Engine::EnginePhase1_InputAndLogic() {
#ifndef NDEBUG
  bus->current_phase = BusPhase::Open;
#endif
  // -----------------------------------------------------------------------------
  //scenePacket scenePkt =
  //    CreateScenePacket(packet->reg, packet->bus, packet->time->gameDt);
  
  Engine::pollEvents();
  
  
  //SceneManager_Update(&scenePkt);
}

void Engine::EnginePhase2_Simulation() {
#ifndef NDEBUG
  bus->current_phase = BusPhase::Simulation;
#endif

  //EntitySystem_Update(&entityPkt);

  while (timeSystem.consumeFixedStep()) {
    //PhysicsSystem_Update(&physicsPkt);
  }
  AnimationSystem_Update();

  audioSystem_Update(*bus);
}

void Engine::EnginePhase3_RenderState() {
#ifndef NDEBUG
  bus->current_phase = BusPhase::Render;
#endif

  //scenePacket scenePkt = CreateScenePacket(packet->reg, packet->bus, packet->time->gameDt);
  //cameraSystem_Update(&camPkt);

  //rendererCore_BeginFrame();
  //SceneManager_Draw(&scenePkt);
  //rendererCore_EndFrame();
}

void Engine::EnginePhase4_Cleanup() {
#ifndef NDEBUG
  bus->current_phase = BusPhase::Open;
#endif
  bus->clear();
}

void Engine::processAllCommands() {
  // xxxSystem_ProcessCommands should be added here!!!
  // though all systems need tons of parameters, might also add this to phases instead of this being standalone
  bus->buildIndex();
  // 
  // 
  // 
  // 
  // 
  bus->flush();
}


bool Engine::init(Arena* masterArena) {
  // SubArena Allocations, check this again, dont like how long it is.
  entityArena = masterArena->Split(static_cast<size_t>(256 * 1024 * 1024), 64);  // 256MB
  physicsArena = masterArena->Split(static_cast<size_t>(4 * 1024 * 1024), 64); // 4MB
  busArena = masterArena->Split(static_cast<size_t>(18 * 1024 * 1024), 64);     // 18MB
  audioArena = masterArena->Split(static_cast<size_t>(16 * 1024 * 1024), 64);  // 16MB
  frameArena = masterArena->Split(static_cast<size_t>(16 * 1024 * 1024), 64);  // 16MB
  // create, allocate, and initialize command bus.
  bus = CommandBus::create(&busArena);
  if (!bus) {
    LogSystem::Error("[ENGINE] CommandBus failed to initialize. ");
    return false;
  }
  // create, allocate, and initialize registry.
  reg = Registry::createRegistry(&entityArena);
  if (!reg) {
    LogSystem::Error("[ENGINE] Registry failed to initialize. ");
    return false;
  }
  
  if (!platform.init()) {
    LogSystem::Error("[ENGINE] Platform failed to initialize. ");
    return false;
  }
  
  // Logger initialization
  LogSystem::init(platform.GetPrefPath());

  timeSystem.init();
  if (!windowSystem.init()) {
    LogSystem::Error("[ENGINE] WindowSystem failed to initialize. ");
    platform.shutdown();
    LogSystem::shutdown();
    return false;
  }
  if (!inputSystem.init()) {
    LogSystem::Error("[ENGINE] InputSystem failed to initialize. ");
    windowSystem.shutdown();
    platform.shutdown();
    LogSystem::shutdown();
    return false;
  }  
  
  //EntityManager_Init(*reg);
  //Asset_Init();

  //rendererCore_Init(v.width, v.height);
  //PhysicsSystem_Init();
  //cameraSystem_Init(*ctx.reg);
  
  
  audioSystem_Init();
  running = true;
  return true;
}

void Engine::pollEvents() {
    SDL_Event event;
    while (platform.pollEvent(event)) {
        inputSystem.handleEvent(event);
        windowSystem.handleEvent(event);
    }
    if (windowSystem.shouldClose()) {
        running = false;
    }
}
void Engine::run() {
  while (running) {
    EnginePhase0_PlatformSync();
     processAllCommands();
     
    EnginePhase1_InputAndLogic();
     processAllCommands();
     
    EnginePhase2_Simulation();
     processAllCommands();
     
    EnginePhase3_RenderState();
     processAllCommands();
     
    EnginePhase4_Cleanup();
    frameArena.Clear();
  }
}
void Engine::shutdown() {
  LogSystem::Info("Engine Shutting down...");
  SceneManager_Shutdown(*reg, *bus);
  //EntityManager_Shutdown(*reg);
  audioSystem_Shutdown();

  // Shutdown should be in reverse order of initialization.
  inputSystem.shutdown();
  windowSystem.shutdown();
  platform.shutdown();
  // Add failsafes later on.
  
  running = false;
  LogSystem::Info("Engine Shutdown Complete.");
  LogSystem::shutdown();
}
