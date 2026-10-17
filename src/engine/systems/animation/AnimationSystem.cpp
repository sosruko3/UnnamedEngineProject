#include "AnimationSystem.h"
#include "engine/core/CommandBus.h"
#include "engine/core/SystemPackets.h"
#include "assert.h"

void AnimationSystem_ProcessCommands(CommandBus& bus) {
  for (const Command& cmd : bus.commandsOf(Domain::Anim)) {
    switch (cmd.type) {
    case CommandType::ANIM_PLAY: {
      break;
    }
    case CommandType::ANIM_STOP: {
      break;
    }
    case CommandType::ANIM_PAUSE: {
      break;
    }
    case CommandType::ANIM_RESUME: {
      break;
    }
    default:
      break;
    }
  }
}

void AnimationSystem_Update() {
}