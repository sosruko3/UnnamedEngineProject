#include "Engine.h"
#include "CommandBus.h"
#include "LogSystem.h"
#include "engine/core/SystemPackets.h"
#include "engine/common/EngineTypes.h"
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/EntitySystem.h"
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
  bus->consumed_end = bus->tail;
#ifndef NDEBUG
  bus->current_phase = BUS_PHASE_OPEN;
#endif
  // -----------------------------------------------------------------------------
  //scenePacket scenePkt =
  //    CreateScenePacket(packet->reg, packet->bus, packet->time->gameDt);
  
  Engine::pollEvents();
  
  
  //SceneManager_Update(&scenePkt);
}

void Engine::EnginePhase2_Simulation() {
#ifndef NDEBUG
  bus->current_phase = BUS_PHASE_SIMULATION;
#endif

  entityPacket entityPkt = CreateEntityPacket(reg, bus);
  //animPacket animPkt = CreateAnimPacket(reg, bus, time->gameDt);

  EntitySystem_Update(&entityPkt);

  while (timeSystem.consumeFixedStep()) {
    //PhysicsSystem_Update(&physicsPkt);
  }
  AnimationSystem_Update();

  audioSystem_Update(*bus);
}

void Engine::EnginePhase3_RenderState() {
#ifndef NDEBUG
  bus->current_phase = BUS_PHASE_RENDER;
#endif

  //scenePacket scenePkt = CreateScenePacket(packet->reg, packet->bus, packet->time->gameDt);
  //cameraSystem_Update(&camPkt);

  //rendererCore_BeginFrame();
  //SceneManager_Draw(&scenePkt);
  //rendererCore_EndFrame();
}

void Engine::EnginePhase4_Cleanup() {
#ifndef NDEBUG
  bus->current_phase = BUS_PHASE_OPEN;
#endif

  uint32_t flush_end = bus->consumed_end;

#ifndef NDEBUG
  assert((flush_end - bus->tail) <=
             (bus->head - bus->tail) &&
         "consumed_end outside [tail..head] range");
#endif

  if ((flush_end - bus->tail) >
      (bus->head - bus->tail)) {
    flush_end = bus->tail;
  }

  CommandIterator iter = {.current = bus->tail, .end = flush_end};
  CommandBus_Flush(*bus, &iter);
}


bool Engine::init(Arena* masterArena) {
  // SubArena Allocations, check this again, dont like how long it is.
  entityArena = masterArena->Split(static_cast<size_t>(40 * 1024 * 1024), 64);  // 40MB
  physicsArena = masterArena->Split(static_cast<size_t>(4 * 1024 * 1024), 64); // 4MB
  busArena = masterArena->Split(static_cast<size_t>(8 * 1024 * 1024), 64);     // 8MB
  audioArena = masterArena->Split(static_cast<size_t>(16 * 1024 * 1024), 64);  // 16MB
  frameArena = masterArena->Split(static_cast<size_t>(16 * 1024 * 1024), 64);  // 16MB
  reg = entityArena.Push<EntityRegistry>();
  bus = busArena.Push<CommandBus>();

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
    platform.shutdown();
    windowSystem.shutdown();
    LogSystem::shutdown();
    return false;
  }
  CommandBus_Init(*bus);
  EntityManager_Init(*reg);
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
    EnginePhase1_InputAndLogic();
    EnginePhase2_Simulation();
    EnginePhase3_RenderState();
    EnginePhase4_Cleanup();
    frameArena.Clear();
  }
}
void Engine::shutdown() {
  LogSystem::Info("Engine Shutting down...");
  SceneManager_Shutdown(*reg, *bus);
  EntityManager_Shutdown(*reg);
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
