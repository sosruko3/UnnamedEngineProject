#pragma once

#include <stdbool.h>
#include <stdint.h>

struct EntityRegistry;
struct CommandBus;
struct animPacket;

void AnimationSystem_ProcessCommands(animPacket *packet);
void AnimationSystem_Update();
