#pragma once
#include <stdbool.h>
#include <stdint.h>

struct EntityRegistry;
struct CommandBus;
struct physicsPacket;

void PhysicsSystem_Init(void);
void PhysicsSystem_Update(physicsPacket *packet);

void PhysicsSystem_ProcessCommands(physicsPacket* packet);
