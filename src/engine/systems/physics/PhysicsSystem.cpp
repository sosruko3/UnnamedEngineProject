#include "PhysicsSystem.h"
#include "engine/core/CommandBus.h"
//#include "engine/core/LogSystem.h"
#include "engine/core/SystemPackets.h"
//#include "engine/common/EngineTypes.h"
//#include "engine/ecs/EntityRegistry.h"
#include <assert.h>
//#include <cstdint>
#include <math.h>

void PhysicsSystem_Init(void) {
}

void PhysicsSystem_Update(physicsPacket *packet) {
  PhysicsSystem_ProcessCommands(packet);
}

void PhysicsSystem_ProcessCommands(physicsPacket *packet) {
  auto bus = packet->bus;
  CommandIterator iter = CommandBus_GetIterator(*bus);
  const Command *cmd;

  while (CommandBus_Next(*bus, &iter, &cmd)) {
    if ((cmd->type & CMD_DOMAIN_MASK) != CMD_DOMAIN_PHYS)
      continue;

    switch (cmd->type) {
      case CMD_PHYS_DEFINE: {
        break;
      }
      case CMD_PHYS_TELEPORT: {
        break;
      }

      case CMD_PHYS_SET_VELOCITY: {
        break;
      }

      case CMD_PHYS_APPLY_IMPULSE: {
        break;
      }

      case CMD_PHYS_SET_DRAG: {
        break;
      }

      case CMD_PHYS_SET_GRAVITY_SCALE: {
        break;
      }
      case CMD_PHYS_SET_GRAVITY: {
        break;
      }
      default:
        // Not a physics command, ignore
        break;
    }
  }
}
