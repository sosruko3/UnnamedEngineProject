#include "AnimationSystem.h"
#include "engine/core/CommandBus.h"
#include "engine/core/SystemPackets.h"
#include "engine/ecs/Registry.h"
#include "assert.h"

void AnimationSystem_ProcessCommands(animPacket *packet,CommandBus* bus) {
  CommandIterator iter = CommandBus_GetIterator(*packet->bus);
  const Command *cmd;

  while (CommandBus_Next(*bus, &iter, &cmd)) {
    if ((cmd->type & CMD_DOMAIN_MASK) != CMD_DOMAIN_ANIM)
      continue;
    // This assumes every anim commands has entity. Change this if you add some
    // global command.
    //if (!EntityRegistry_IsAlive(flags, generations, cmd->entity))
    //  continue;
    //const Entity entity = cmd->entity;
    //const uint32_t id = entity.id;
    //if (!(masks[id] & COMP_ANIMATION))
    // continue;

    switch (cmd->type) {
    case CMD_ANIM_PLAY: {
      break;
    }
    case CMD_ANIM_STOP: {
      break;
    }
    case CMD_ANIM_PAUSE: {
      break;
    }
    case CMD_ANIM_RESUME: {
      break;
    }

    default:
      break;
    }
  }
}

void AnimationSystem_Update() {
}