#include "Arena.h"
#include "engine/core/LogSystem.h"
#include <cstdlib>
#include <assert.h>
#include <stdio.h>

// using c++ and C libraries mixed is not good. fix this sometime. choose one and use them.

Arena ArenaAllocate(size_t capacity) {
    assert(capacity > 0);
    assert(capacity <= (SIZE_MAX - (MASTER_ARENA_ALIGNMENT - 1)) && "[ARENA] Requested Allocation size too large!");
    size_t aligned_capacity = (capacity + MASTER_ARENA_ALIGNMENT - 1) & ~(MASTER_ARENA_ALIGNMENT - 1);
    void* raw_memory = std::aligned_alloc(MASTER_ARENA_ALIGNMENT, aligned_capacity);
    if (!raw_memory) {
        Log(LogLevel::Error, "[ARENA] Failed to allocate memory!");
        return {};
    }

    Arena arena = {};
    arena.Bind(raw_memory, aligned_capacity);
    return arena;
}

void ArenaFree(Arena* arena) {
    if (!arena || !(arena->base_ptr)) return;
    std::free(arena->base_ptr);
    *arena = {};
}

void Arena::Bind(void* memory, size_t cap) {
    assert(memory != nullptr);
    base_ptr = static_cast<uint8_t*>(memory);
    capacity = cap;
    offset = 0;
}

void Arena::Clear() {
    offset = 0;
}

size_t Arena::Mark() const {
    return offset;
}

void Arena::Rewind(size_t snapshot) {
    assert(snapshot <= offset && "[ARENA] No forward rewind allowed!");
    offset = snapshot;
}

size_t Arena::Remaining() const {
    return capacity - offset;
}

Arena Arena::Split(size_t split_size, size_t alignment) {
    void* child_memory = PushAligned(split_size, alignment);
    
    memset(child_memory, 0, split_size); // Zero out the memory
    Arena child_arena = {};
    child_arena.Bind(child_memory, split_size);
    return child_arena;
}

void* Arena::PushAligned(size_t size, size_t alignment) {
    assert(alignment != 0 && (alignment & (alignment - 1)) == 0
    && "Alignment should be power of 2");
    assert(offset <= capacity);

    uintptr_t current_addr = reinterpret_cast<uintptr_t>(base_ptr + offset);

    size_t padding = 0;
    size_t modulo = current_addr & (alignment - 1);
    if (modulo != 0)
        padding = alignment - modulo;
    
    size_t total_needed = size + padding;
    assert(total_needed >= size && "Size_t Overflow detected!");
    assert(total_needed <= (capacity - offset) && "Arena is out of memory!");
    void* aligned_memory = reinterpret_cast<void*>(base_ptr + offset + padding);
    offset += total_needed;
    return aligned_memory;
}
