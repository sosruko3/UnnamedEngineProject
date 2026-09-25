#include "EntityManager.h"
#include "engine/core/LogSystem.h"
#include "engine/common/EngineTypes.h"
#include "engine/memory/Arena.h"
#include <assert.h>

// Using FIFO Ring buffer on the free list.
// Gen=0 invalid, (1,3,5,...)Odd gens are dead, (2,4,6,...)Even gens are alive excluding 0.
// Using a fixed-size array for generations and free indices.
// Custom allocation using Arena, no dynamic memory allocation.
// destroy() is deferred. An entity is alive until the command bus is flushed. isValid() reflects this.
// add assert for gen overflow.
constexpr uint32_t MAX_ENTITIES_MASK = MAX_ENTITIES - 1;


bool EntityManager::init(Arena* entityArena) {
  if (!entityArena) {
    LogSystem::Error("Entity Manager: Entity Arena is null");
    return false;
  }
  generations = entityArena->Push<uint32_t>(MAX_ENTITIES);
  free_indices = entityArena->Push<uint32_t>(MAX_ENTITIES);
  // fill generations with 1
  for (uint32_t i = 0; i < MAX_ENTITIES; i++) {
    generations[i] = 1;
  }
  head = 0;
  tail = 0;
  recycled_count = 0;
  next_fresh = 0;
  LogSystem::Info("Entity Manager initialized");
  return true;
}

Entity EntityManager::create() {
  uint32_t index;
  if (recycled_count > 0) {
    // reuse a free entity
    index = free_indices[head];
    head = (head + 1) & MAX_ENTITIES_MASK;
    recycled_count--;
  } else if (next_fresh < MAX_ENTITIES) {
    // allocate a new entity
    index = next_fresh++;
  } else {
    // no more entities available
    return Entity{};
  }
  // increment generation and return entity
  return Entity{index | (++generations[index] << ENTITY_INDEX_BITS)};
}

void EntityManager::destroy(Entity e) {
  // if stale, skip
  if (e.index() >= MAX_ENTITIES) 
    return;
  if (generations[e.index()] != e.generation()) 
    return;
  if (e.generation() % 2 == 1) 
    return; // odd gen = dead, even = alive, 0 = invalid
  
  free_indices[tail] = e.index();
  // increment tail and recycled count
  tail = (tail + 1) & MAX_ENTITIES_MASK;
  recycled_count++;
  // gen is incremented both when created and destroyed
  // odd gen = dead, even = alive, 0 = invalid
  generations[e.index()]++;
}
bool EntityManager::isValid(Entity e) const {
  // if no recycled entities and next_fresh is max, entity is invalid
  if (e.index() >= MAX_ENTITIES) 
    return false;
  if (e.generation() == 0) 
    return false;  // generation 0 is invalid
  return generations[e.index()] == e.generation();
}
uint32_t EntityManager::count() const {
  return (next_fresh - recycled_count);
}

void EntityManager::shutdown() {
  generations = nullptr;
  free_indices = nullptr;
  head = 0;
  tail = 0;
  recycled_count = 0;
  next_fresh = 0;
  LogSystem::Info("Entity Manager shutdown");
}
