#pragma once
#include <stdint.h>

struct Entity;
struct Arena;

struct EntityManager {
    uint32_t *generations = nullptr;
    uint32_t *free_indices = nullptr;
    uint32_t head = 0;
    uint32_t tail = 0;
    uint32_t recycled_count = 0; // number of recycled entities
    uint32_t next_fresh = 0; // next entity to be created

    bool init(Arena* entityArena);
    Entity create(); // instant
    void destroy(Entity e); 
    bool isValid(Entity e) const;
    uint32_t count() const;
    void clear(); // skip implementation for now.
    void shutdown();
};