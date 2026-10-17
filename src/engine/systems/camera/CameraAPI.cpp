#include "CameraAPI.h"
#include "engine/core/CommandBus.h"
#include "engine/core/LogSystem.h"
#include <assert.h>

void cameraAPI_SetActive(CommandBus &bus, Entity cameraEntity, bool isActive) {
  Command cmd = {
      .type = CommandType::CAM_SET_ACTIVE,
      .entity = cameraEntity,
      .b8 = {.value = isActive},
  };
  if (!bus.push(cmd, Domain::Camera, Order::Normal)) {
    LogSystem::Error("Failed to push CAM_SET_ACTIVE command");
  }
}

void cameraAPI_SetPriority(CommandBus &bus, Entity cameraEntity,
                           uint16_t priority) {
  Command cmd = {
      .type = CommandType::CAM_SET_PRIORITY,
      .entity = cameraEntity,
      .u16 = {.value = priority},
  };
  if (!bus.push(cmd, Domain::Camera, Order::Normal)) {
    LogSystem::Error("Failed to push CAM_SET_PRIORITY command");
  }
}

void cameraAPI_SetZoom(CommandBus &bus, Entity cameraEntity, float zoom) {
  Command cmd = {
      .type = CommandType::CAM_SET_ZOOM,
      .entity = cameraEntity,
      .f32 = {.value = zoom},
  };
  if (!bus.push(cmd, Domain::Camera, Order::Normal)) {
    LogSystem::Error("Failed to push CAM_SET_ZOOM command");
  }
}

void cameraAPI_SetRotation(CommandBus &bus, Entity cameraEntity,
                           float rotation) {
  Command cmd = {
      .type = CommandType::CAM_SET_ROTATION,
      .entity = cameraEntity,
      .f32 = {.value = rotation},
  };
  if (!bus.push(cmd, Domain::Camera, Order::Normal)) {
    LogSystem::Error("Failed to push CAM_SET_ROTATION command");
  }
}

void cameraAPI_SetFollowTarget(CommandBus &bus, Entity cameraEntity,
                               Entity targetEntity, float smoothSpeed,
                               creVec2 offset) {
  Command cmd = {
      .type = CommandType::CAM_SET_FOLLOW,
      .entity = cameraEntity,
      .camFollow =
          {
              .targetEntity = targetEntity,
              .smoothSpeed = smoothSpeed,
              .offset = offset,
          },
  };
  if (!bus.push(cmd, Domain::Camera, Order::Normal)) {
    LogSystem::Error("Failed to push CAM_SET_FOLLOW command");
  }
}

void cameraAPI_DisableFollow(CommandBus &bus, Entity cameraEntity) {
  Command cmd = {
      .type = CommandType::CAM_DISABLE_FOLLOW,
      .entity = cameraEntity,
      .u16 = {},
  };
  if (!bus.push(cmd, Domain::Camera, Order::Normal)) {
    LogSystem::Error("Failed to push CAM_DISABLE_FOLLOW command");
  }
}
