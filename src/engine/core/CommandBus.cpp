#include "CommandBus.h"
#include "LogSystem.h"
#include <assert.h>
#include <new>

// This CommandBus architecture has some architectural contracts:
// 0. Designed to be simple and easy to use, not breaking that for extra features
// 1. Flushes are aggresive, flushing all commands regardless of their domain or order
// 2. Flushes are done from previous flushed count to current count
// These means, we can't easily hide/protect a command from being flushed
// Which is accepted tradeoff.
// 3. removed count variable, using this->commands.size() instead

CommandBus* CommandBus::create(Arena* arena) {
  if (arena == nullptr) return nullptr;
  // Allocate memory and initialize the CommandBus.
  auto* storage = arena->Push<CommandBus>();
  auto* bus = ::new (storage) CommandBus{};
  if (!bus) return nullptr;
  // Initialize the CommandBus buffer and variables.
  if (!bus->init(arena)) {
    ::delete bus;
    return nullptr;
  }
  return bus;
}

bool CommandBus::init(Arena* arena) {
  assert(arena != nullptr && "CommandBus::init requires an arena");
  // initialized with COMMAND_CAP capacity, domains and orders will resize to .count() at flush time
  if (!this->commands.init(arena, COMMAND_CAP))
    return false;
  if (!this->domains.init(arena, COMMAND_CAP))
    return false;
  if (!this->orders.init(arena, COMMAND_CAP))
    return false;
  // this calls init to 2 more arenaArrays internally, be aware.
  if (!this->index.init(arena, COMMAND_CAP))
    return false;
  
  this->_flushedCount = 0;
#ifndef NDEBUG
  this->current_phase = BusPhase::Open;
  this->debug_forbidden_domain = Domain::None;
#endif
  return true;
}

void CommandBus::flush() {
  assert(this->_flushedCount <= this->commands.size());
  assert(this->commands.size() == this->domains.size());
  assert(this->commands.size() == this->orders.size());
  if (this->commands.size() >= 10'000) {
    // Log a warning if the command count exceeds 10,000.
    LogSystem::Warning("CommandBus flushing, command count is {}", this->commands.size());
  }
  
  // Advance _flushedCount to current count
  this->_flushedCount = this->commands.size();
}

void CommandBus::clear() {
  this->commands.resize(0);
  this->domains.resize(0);
  this->orders.resize(0);
  this->_flushedCount = 0;
#ifndef NDEBUG
  this->current_phase = BusPhase::Open;
  this->debug_forbidden_domain = Domain::None;
#endif
}

bool CommandBus::push(const Command& cmd, Domain domain, Order order) {
#ifndef NDEBUG
// these are staying for debugging purposes.
  if (this->current_phase == BusPhase::Render) {
    assert(domain == Domain::Render || cmd.type == CommandType::NONE);
  }
  assert(this->debug_forbidden_domain == Domain::None ||
         domain != this->debug_forbidden_domain);
#endif

  if (this->isFull()) {
    LogSystem::Error("Command bus buffer is full; command dropped");
    return false;
  }

  assert(domain < Domain::Count);
  assert(order < Order::Count);
  assert(this->commands.size() == this->domains.size());
  assert(this->commands.size() == this->orders.size());
  // fill arrays on ::push, so that we can do counting sort on indexing.
  this->domains.push(domain);
  this->orders.push(order);
  this->commands.push(cmd);
  return true;
}

void CommandBus::buildIndex() {
  this->index.build(this->domains.data(), this->orders.data(), _flushedCount, this->domains.size());
}

// returns the commands for the given domain.
const IndexedView<Command> CommandBus::commandsOf(Domain domain) const {
  return this->index.viewOf(domain, this->commands.data());
}

uint32_t CommandBus::count() const {
  return this->domains.size();
}

bool CommandBus::isEmpty() const {
  return this->domains.empty();
}

bool CommandBus::isFull() const {
  return this->domains.size() >= COMMAND_CAP;
}

uint32_t CommandBus::remaining() const {
  return this->domains.size() - this->_flushedCount;
}

bool CommandBus::isDone() const {
  return this->_flushedCount == this->domains.size();
}
