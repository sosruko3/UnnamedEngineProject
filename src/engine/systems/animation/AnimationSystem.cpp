#include "AnimationSystem.h"
#include "engine/core/CommandBus.h"
#include "engine/core/SystemPackets.h"
#include "engine/ecs/EntityRegistry.h"
#include "assert.h"

animPacket CreateAnimPacket(EntityRegistry *reg, CommandBus *bus, float dt) {
  animPacket pkt = {
      .bus = bus,
      .dt = dt,
      .max_used_bound = reg->max_used_bound,
      .read = {.component_masks = reg->component_masks,
               .generations = reg->generations},
      .write = {.state_flags = reg->state_flags,
                .sprite_ids = reg->sprite_ids,
                .anim_timers = reg->anim_timers,
                .anim_speeds = reg->anim_speeds,
                .anim_ids = reg->anim_ids,
                .anim_frames = reg->anim_frames,
                .anim_finished = reg->anim_finished,
                .anim_base_durations = reg->anim_base_durations,
                .anim_frame_counts = reg->anim_frame_counts,
                .anim_start_sprites = reg->anim_start_sprites,
                .anim_loops = reg->anim_loops},
  };
  return pkt;
}

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