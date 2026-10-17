#include "AnimationAPI.h"
#include "engine/core/CommandBus.h"
#include "engine/core/LogSystem.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

void animAPI_Play(CommandBus &bus, Entity entity, uint16_t animID,
                  bool forceReset) {

  uint16_t flags = forceReset ? ANIM_FLAG_FORCE_RESET : 0;
  Command cmd = {.type = CommandType::ANIM_PLAY,
                 .entity = entity,
                 .anim = {.animID = animID, .flags = flags}};
  if (!bus.push(cmd, Domain::Anim, Order::Normal)) {
    LogSystem::Warning("Failed to push ANIM_PLAY command");
  }
}

void animAPI_Stop(CommandBus &bus, Entity entity) {
  Command cmd = {
      .type = CommandType::ANIM_STOP,
      .entity = entity,
      .u16 = {},
  };
  if (!bus.push(cmd, Domain::Anim, Order::Normal)) {
    LogSystem::Warning("Failed to push ANIM_STOP command");
  }
}

void animAPI_Resume(CommandBus &bus, Entity entity) {
  Command cmd = {
      .type = CommandType::ANIM_RESUME,
      .entity = entity,
      .u16 = {},
  };
  if (!bus.push(cmd, Domain::Anim, Order::Normal)) {
    LogSystem::Warning("Failed to push ANIM_RESUME command");
  }
}

void animAPI_Pause(CommandBus &bus, Entity entity) {
  Command cmd = {
      .type = CommandType::ANIM_PAUSE,
      .entity = entity,
      .u16 = {},
  };
  if (!bus.push(cmd, Domain::Anim, Order::Normal)) {
    LogSystem::Warning("Failed to push ANIM_PAUSE command");
  }
}

void animAPI_SetSpeed(CommandBus &bus, Entity entity, float speed) {
  Command cmd = {
      .type = CommandType::ANIM_SET_SPEED, .entity = entity, .f32 = {.value = speed}};
  if (!bus.push(cmd, Domain::Anim, Order::Normal)) {
    LogSystem::Warning("Failed to push ANIM_SET_SPEED command");
  }
}

void animAPI_SetFrame(CommandBus &bus, Entity entity, uint16_t frame) {
  Command cmd = {
      .type = CommandType::ANIM_SET_FRAME, .entity = entity, .u16 = {.value = frame}};
  if (!bus.push(cmd, Domain::Anim, Order::Normal)) {
    LogSystem::Warning("Failed to push ANIM_SET_FRAME command");
  }
}

void animAPI_SetLoop(CommandBus &bus, Entity entity, bool loop) {
  Command cmd = {
      .type = CommandType::ANIM_SET_LOOP, .entity = entity, .b8 = {.value = loop}};
  if (!bus.push(cmd, Domain::Anim, Order::Normal)) {
    LogSystem::Warning("Failed to push ANIM_SET_LOOP command");
  }
}
