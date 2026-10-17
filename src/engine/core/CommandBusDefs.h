#pragma once

#include "engine/common/EngineTypes.h"
#include <stdbool.h>
#include <stdint.h>

// Order enum
enum class Order : uint8_t {
  start = 0, // placeholder for id 0
  Early = 32,
  Normal = 128,
  Late = 224,
  Count = 255,
};

// Domain enum
enum class Domain : uint8_t { 
  None = 0, 
  Phys, 
  Ecs, 
  Anim,
  Render, 
  Camera,
  Audio, 
  Count,
};
constexpr uint32_t DomainCount = static_cast<uint32_t>(Domain::Count);


// Check this part again, can be done in a lot more efficient way, not really loving it
// Give domain's base value
constexpr uint32_t CMD_DOMAIN_SHIFT = 12;
constexpr uint32_t DomainBase(Domain d) { return static_cast<uint32_t>(d) << CMD_DOMAIN_SHIFT; }
// Domains
constexpr uint32_t DOMAIN_PHYS = DomainBase(Domain::Phys);
constexpr uint32_t DOMAIN_ECS  = DomainBase(Domain::Ecs);
constexpr uint32_t DOMAIN_ANIM = DomainBase(Domain::Anim);
constexpr uint32_t DOMAIN_RENDER = DomainBase(Domain::Render);
constexpr uint32_t DOMAIN_CAMERA = DomainBase(Domain::Camera);
constexpr uint32_t DOMAIN_AUDIO = DomainBase(Domain::Audio);

// Command Flags
// Remove these flags later on!
constexpr uint8_t CMD_PHYS_FLAG_STATIC = (1 << 0); // Bit 0: Wall/Static
constexpr uint8_t ANIM_FLAG_FORCE_RESET = (1 << 0); // Bit 0: Force Reset

// Command Types
// *_COUNT wastes 2xDomain enum slot for per-domain used command count
// not urgent since each domain has 4k command slot.
enum class CommandType : uint32_t {
  // domain-agnostic, generic? commands, not sure if need them
  NONE = 0,
  
  // Physics commands
  PHYS_TELEPORT = DOMAIN_PHYS,
  PHYS_SET_VELOCITY,
  PHYS_LOAD_STATIC,
  PHYS_DEFINE,
  PHYS_RESET,
  PHYS_APPLY_IMPULSE,
  PHYS_SET_DRAG,
  PHYS_SET_GRAVITY_SCALE,
  PHYS_SET_MATERIAL,
  PHYS_SET_GRAVITY,
  PHYS_LAST,
  PHYS_COUNT = PHYS_LAST - DOMAIN_PHYS, // wasting two slots here, this is temp fix,

  // Entity commands
  // Change these to ECS_SPAWN etc.
  ENTITY_SPAWN = DOMAIN_ECS,
  ENTITY_SPAWN_UNTRACKED,
  ENTITY_CLONE,
  ENTITY_DESTROY,
  ENTITY_ADD_COMPONENT,
  ENTITY_REMOVE_COMPONENT,
  ENTITY_SET_PIVOT,
  ENTITY_SET_TYPE,
  ENTITY_ADD_FLAGS,
  ENTITY_REMOVE_FLAGS,
  ENTITY_RESET,
  ECS_LAST,
  ECS_COUNT = ECS_LAST - DOMAIN_ECS,

  // Animation commands
  ANIM_PLAY = DOMAIN_ANIM,
  ANIM_STOP,
  ANIM_PAUSE,
  ANIM_RESUME,
  ANIM_SET_SPEED,
  ANIM_SET_FRAME,
  ANIM_SET_LOOP,
  ANIM_LAST,
  ANIM_COUNT = ANIM_LAST - DOMAIN_ANIM,

  // Render commands
  RENDER_SETDEPTHMATH = DOMAIN_RENDER,
  RENDER_SET_SPRITE,
  RENDER_SET_COLOR,
  RENDER_SET_VISIBLE,
  RENDER_SET_VISUAL_SCALE,
  RENDER_SET_ROTATION,
  RENDER_SET_LAYER,
  RENDER_LAST,
  RENDER_COUNT = RENDER_LAST - DOMAIN_RENDER,

  // Camera commands
  CAM_SET_ACTIVE = DOMAIN_CAMERA,
  CAM_SET_PRIORITY,
  CAM_SET_ZOOM,
  CAM_SET_ROTATION,
  CAM_SET_FOLLOW,
  CAM_DISABLE_FOLLOW,
  CAM_LAST,
  CAM_COUNT = CAM_LAST - DOMAIN_CAMERA,

  // Audio commands
  AUDIO_GROUP_INIT = DOMAIN_AUDIO,
  AUDIO_SET_MASTER_VOLUME,
  AUDIO_SET_LISTENER_POSITION,
  AUDIO_GROUP_SET_VOLUME,
  AUDIO_GROUP_SET_PITCH,
  AUDIO_GROUP_SET_PAN,
  AUDIO_PLAY_ONESHOT,
  AUDIO_SOUND_LOAD,
  AUDIO_SOUND_UNLOAD,
  AUDIO_SOUND_PLAY,
  AUDIO_SOUND_PAUSE,
  AUDIO_SOUND_STOP,
  AUDIO_SOUND_SET_VOLUME,
  AUDIO_SOUND_SET_PITCH,
  AUDIO_SOUND_SET_PAN,
  AUDIO_SOUND_SET_LOOPING,
  AUDIO_SOUND_SET_SPATIALIZATION,
  AUDIO_SOUND_SET_POSITION,
  AUDIO_SOUND_SET_ATTENUATION,
  AUDIO_LAST,
  AUDIO_COUNT = AUDIO_LAST - DOMAIN_AUDIO,
};

// Command Payloads (4-byte aligned)
// These are generic payloads that are used by many commands
// Heavily favored over specific payloads when possible
typedef struct {
  creVec2 value;
} CommandPayloadVec2;

typedef struct {
  float value;
} CommandPayloadF32;

typedef struct {
  uint16_t value;
} CommandPayloadU16;

typedef struct {
  bool value;
} CommandPayloadB8;

typedef struct {
  uint8_t value;
} CommandPayloadU8;

typedef struct {
  uint64_t value;
} CommandPayloadU64;

// Specific Payloads
// Used only when a command has a highly unique footprint

typedef struct {
  uint16_t animID;
  uint16_t flags;
} CommandPayloadAnim;

typedef struct {
  uint8_t material_id; // e.g., MAT_WOOF
  uint8_t flags;       // e.g., static,
  // removed 2 bytes of _padding
  float drag;
} CommandPayloadPhysDef;

typedef struct {
  Entity prototype;
  creVec2 position;
} CommandPayloadEntityClone;

typedef struct {
  Entity targetEntity;
  float smoothSpeed;
  creVec2 offset;
} CommandPayloadCamFollow;

typedef struct {
  creColor color;
} CommandPayloadColor;

typedef struct {
  float wX;
  float wY;
  float wH;
  uint8_t shiftBatch;
  uint8_t shiftDepth;
  // removed 2 bytes of _padding
} CommandPayloadRenderDepth;

typedef struct {
  AudioID id;
} CommandPayloadAudioID;

typedef struct {
  AudioID id;
  AudioSourceID sourceID;
  AudioGroupID groupID;
  AudioUsageType usageType;
} CommandPayloadAudioLoad;

typedef struct {
  AudioGroupID groupID;
  // removed 3 bytes of _padding
  float value;
} CommandPayloadAudioGroup;

typedef struct {
  AudioID id;
  float value;
} CommandPayloadAudioF32;

typedef struct {
  AudioID id;
  bool value;
  // removed 3 byte of _padding
} CommandPayloadAudioB8;

typedef struct {
  AudioID id;
  creVec2 value;
} CommandPayloadAudioVec2;

typedef struct {
  AudioSourceID sourceid;
  AudioGroupID groupid;
  // removed 1 byte of _padding
} CommandPayloadAudioOneShot;
