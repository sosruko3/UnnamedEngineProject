#pragma once

#include <stdbool.h>
#include <stdint.h>

struct EntityRegistry;
struct CommandBus;

void AnimationSystem_ProcessCommands(CommandBus& bus);
void AnimationSystem_Update();
