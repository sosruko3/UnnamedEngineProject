#include "AudioAPI.h"
#include "engine/core/CommandBus.h"
#include "engine/core/LogSystem.h"
#include "engine/common/EngineTypes.h"
#include <assert.h>

AudioID audioAPI_AllocateSound(void) { return audioSystem_AllocateID(); }

void audioAPI_GroupInit(CommandBus &bus, AudioGroupID groupID) {
  Command cmd = {
      .type = CommandType::AUDIO_GROUP_INIT,
      .entity = Entity{},
      .u8 = {.value = groupID},
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_GroupInit: CommandBus is full!");
  }
}

void audioAPI_SetMasterVolume(CommandBus &bus, float volume) {
  Command cmd = {
      .type = CommandType::AUDIO_SET_MASTER_VOLUME,
      .entity = Entity{},
      .f32 = {.value = volume},
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SetMasterVolume: CommandBus is full!");
  }
}

void audioAPI_SetListenerPosition(CommandBus &bus, creVec2 position) {
  Command cmd = {
      .type = CommandType::AUDIO_SET_LISTENER_POSITION,
      .entity = Entity{},
      .vec2 = {.value = position},
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SetListenerPosition: CommandBus is full!");
  }
}

void audioAPI_GroupSetVolume(CommandBus &bus, AudioGroupID groupID,
                             float volume) {
  Command cmd = {
      .type = CommandType::AUDIO_GROUP_SET_VOLUME,
      .entity = Entity{},
      .audiogroup =
          {
              .groupID = groupID,
              .value = volume,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_GroupSetVolume: CommandBus is full!");
  }
}

void audioAPI_GroupSetPitch(CommandBus &bus, AudioGroupID groupID,
                            float pitch) {
  Command cmd = {
      .type = CommandType::AUDIO_GROUP_SET_PITCH,
      .entity = Entity{},
      .audiogroup =
          {
              .groupID = groupID,
              .value = pitch,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_GroupSetPitch: CommandBus is full!");
  }
}

void audioAPI_GroupSetPan(CommandBus &bus, AudioGroupID groupID, float pan) {
  Command cmd = {
      .type = CommandType::AUDIO_GROUP_SET_PAN,
      .entity = Entity{},
      .audiogroup =
          {
              .groupID = groupID,
              .value = pan,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_GroupSetPan: CommandBus is full!");
  }
}

void audioAPI_PlayOneShot(CommandBus &bus, AudioGroupID groupID,
                          AudioSourceID sourceID) {
  Command cmd = {
      .type = CommandType::AUDIO_PLAY_ONESHOT,
      .entity = Entity{},
      .audioshot =
          {
              .sourceid = sourceID,
              .groupid = groupID,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_PlayOneShot: CommandBus is full!");
  }
}

void audioAPI_SoundLoad(CommandBus &bus, AudioID id, AudioSourceID sourceID,
                        AudioGroupID groupID, AudioUsageType usageType) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_LOAD,
      .entity = Entity{},
      .audioload =
          {
              .id = id,
              .sourceID = sourceID,
              .groupID = groupID,
              .usageType = usageType,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundLoad: CommandBus is full!");
  }
}

void audioAPI_SoundUnload(CommandBus &bus, AudioID id) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_UNLOAD,
      .entity = Entity{},
      .audioid = {.id = id},
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundUnload: CommandBus is full!");
  }
}

void audioAPI_SoundPlay(CommandBus &bus, AudioID id) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_PLAY,
      .entity = Entity{},
      .audioid = {.id = id},
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundPlay: CommandBus is full!");
  }
}

void audioAPI_SoundPause(CommandBus &bus, AudioID id) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_PAUSE,
      .entity = Entity{},
      .audioid = {.id = id},
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundPause: CommandBus is full!");
  }
}

void audioAPI_SoundStop(CommandBus &bus, AudioID id) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_STOP,
      .entity = Entity{},
      .audioid = {.id = id},
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundStop: CommandBus is full!");
  }
}

void audioAPI_SoundSetVolume(CommandBus &bus, AudioID id, float volume) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_SET_VOLUME,
      .entity = Entity{},
      .audiof32 =
          {
              .id = id,
              .value = volume,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundSetVolume: CommandBus is full!");
  }
}

void audioAPI_SoundSetPitch(CommandBus &bus, AudioID id, float pitch) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_SET_PITCH,
      .entity = Entity{},
      .audiof32 =
          {
              .id = id,
              .value = pitch,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundSetPan: CommandBus is full!");
  }
}

void audioAPI_SoundSetPan(CommandBus &bus, AudioID id, float pan) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_SET_PAN,
      .entity = Entity{},
      .audiof32 =
          {
              .id = id,
              .value = pan,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundSetLooping: CommandBus is full!");
  }
}

void audioAPI_SoundSetLooping(CommandBus &bus, AudioID id, bool looping) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_SET_LOOPING,
      .entity = Entity{},
      .audiob8 =
          {
              .id = id,
              .value = looping,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundSetLooping: CommandBus is full!");
  }
}

void audioAPI_SoundSetSpatialization(CommandBus &bus, AudioID id,
                                     bool enabled) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_SET_SPATIALIZATION,
      .entity = Entity{},
      .audiob8 =
          {
              .id = id,
              .value = enabled,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundSetSpatialization: CommandBus is full!");
  }
}

void audioAPI_SoundSetPosition(CommandBus &bus, AudioID id, creVec2 position) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_SET_POSITION,
      .entity = Entity{},
      .audiovec2 =
          {
              .id = id,
              .value = position,
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundSetPosition: CommandBus is full!");
  }
}

void audioAPI_SoundSetAttenuation(CommandBus &bus, AudioID id,
                                  float minDistance, float maxDistance) {
  Command cmd = {
      .type = CommandType::AUDIO_SOUND_SET_ATTENUATION,
      .entity = Entity{},
      .audiovec2 =
          {
              .id = id,
              .value = {.x = minDistance, .y = maxDistance},
          },
  };

  if (!bus.push(cmd, Domain::Audio, Order::Normal)) {
    LogSystem::Warning("audioAPI_SoundSetAttenuation: CommandBus is full!");
  }
}
