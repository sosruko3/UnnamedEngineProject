#include "PhysicsSystem.h"
#include "engine/core/CommandBus.h"
#include "engine/core/SystemPackets.h"

void PhysicsSystem_Init(void) {
}

void PhysicsSystem_Update(physicsPacket *packet) {
  PhysicsSystem_ProcessCommands(packet);
}

void PhysicsSystem_ProcessCommands(physicsPacket *packet) {
  CommandBus& bus = *packet->bus;
  for (const Command& cmd : bus.commandsOf(Domain::Phys)) {
    switch (cmd.type) {
      case CommandType::PHYS_DEFINE: {
        break;
      }
      case CommandType::PHYS_TELEPORT: {
        break;
      }

      case CommandType::PHYS_SET_VELOCITY: {
        break;
      }

      case CommandType::PHYS_APPLY_IMPULSE: {
        break;
      }

      case CommandType::PHYS_SET_DRAG: {
        break;
      }

      case CommandType::PHYS_SET_GRAVITY_SCALE: {
        break;
      }
      case CommandType::PHYS_SET_GRAVITY: {
        break;
      }
      default:
        // Not a physics command, ignore
        break;
    }
  }
}
