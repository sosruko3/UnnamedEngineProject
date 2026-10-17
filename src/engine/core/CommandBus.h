#pragma once

#include "CommandBusDefs.h"
#include "CommandIndex.h"
#include "engine/common/EngineTypes.h"
#include "engine/memory/Arena.h"
#include "engine/common/ArenaArray.h"
#include <assert.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//  We also have some removed _paddings on Defs.h file, also check it.
//  For last, also check why compiler gives errors for CommandBus{} part on Engine.cpp.
//  And refactor API files for each system for consistency and correct usage.

// Config
constexpr uint32_t COMMAND_CAP = 32768; // safe cap, >10k is very unlikely
constexpr uint32_t COMMAND_CAP_MASK = (COMMAND_CAP - 1);
static_assert((COMMAND_CAP & COMMAND_CAP_MASK) == 0,
              "COMMAND_CAP must be a power of 2");
constexpr uint32_t CMD_BUFFER_ALIGNMENT = 64;


enum class BusPhase : uint8_t {
  Open = 0,
  Simulation = 1,
  Render = 2,
};

struct Command {
  CommandType type; // CommandType (4 bytes)
  Entity entity; // Target entity (id,gen)(4 bytes)
  // Anonymous union - access directly: cmd.move.x, cmd.anim.animID
  union {
    // Generic payloads
    CommandPayloadVec2 vec2;
    CommandPayloadF32 f32;
    CommandPayloadU16 u16;
    CommandPayloadB8 b8;
    CommandPayloadU8 u8;
    CommandPayloadU64 u64;

    // More specific payloads, create one here if generics are not enough.
    CommandPayloadAnim anim;
    CommandPayloadPhysDef physDef;
    CommandPayloadEntityClone entityClone;
    CommandPayloadCamFollow camFollow;
    CommandPayloadColor color;
    CommandPayloadRenderDepth renderDepth;
    CommandPayloadAudioID audioid;
    CommandPayloadAudioLoad audioload;
    CommandPayloadAudioGroup audiogroup;
    CommandPayloadAudioF32 audiof32;
    CommandPayloadAudioB8 audiob8;
    CommandPayloadAudioVec2 audiovec2;
    CommandPayloadAudioOneShot audioshot;
    alignas(4) uint8_t raw[56];
  };
};

// Compile-time struct verification
static_assert(sizeof(Command) == 64, "Command struct must be exactly 64 bytes");
static_assert(alignof(Command) >= 4,
              "Command struct must be at least 4-byte aligned");


// Command Bus
struct CommandBus {
  alignas(64) ArenaArray<Command> commands;
  ArenaArray<Domain> domains;
  ArenaArray<Order> orders;
  CommandIndex index;
  uint32_t _flushedCount = 0;
#ifndef NDEBUG
  BusPhase current_phase = BusPhase::Open;
  Domain debug_forbidden_domain = Domain::None;
#endif

  // Creates a new CommandBus instance on the given arena.
  // also calls init() to initialize the buffer.
  static CommandBus* create(Arena* arena);
  // initializes the buffer on the given arena.
  // also sets _count and _flushedCount to 0.
  [[nodiscard]] bool init(Arena* arena);
  // flushes the buffer
  void flush();
  // sets _count and _flushedCount to 0.
  void clear();
  // pushes a command to the buffer, returning false if the buffer is full.
  [[nodiscard]] bool push(const Command& cmd, Domain domain, Order order);

  // builds the command index from the current buffer contents.
  void buildIndex();

  // returns the commands for the given domain.
  const IndexedView<Command> commandsOf(Domain domain) const;

  // returns the number of commands in the buffer.
  uint32_t count() const;
  bool isEmpty() const;
  bool isFull() const;
  uint32_t remaining() const;
  bool isDone() const;
};
