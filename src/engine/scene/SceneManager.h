#pragma once

#include "EngineScenes.h"
#include <stdint.h>

struct EntityRegistry;
struct CommandBus;
struct scenePacket;

typedef Scene (*SceneFactory)(int32_t stateID);


void SceneManager_Init(SceneFactory factory);

// Main loop calls these
void SceneManager_Update(scenePacket *packet);
void SceneManager_Draw(scenePacket *packet);

// Clean up whatever scene is currently active
void SceneManager_Shutdown(EntityRegistry &reg, CommandBus &bus);

// Switch scenes
void SceneManager_ChangeScene(int32_t nextState);

int SceneManager_GetActiveState(void);
