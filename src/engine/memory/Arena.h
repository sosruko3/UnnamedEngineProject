#pragma once

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

constexpr size_t DEFAULT_ARENA_ALIGNMENT = 8;
constexpr size_t MASTER_ARENA_ALIGNMENT = 64;


struct Arena {
    uint8_t* base_ptr = nullptr;
    size_t   capacity = 0;
    size_t   offset   = 0;

    // Point this arena at an existing block of memory. Does not allocate.
    // For master arena, called by ArenaAllocate. For sub-arenas, called by Split.
    void Bind(void* memory, size_t cap);

    // Reset offset to 0. Does not free memory. Use to reuse the arena.
    void Clear();

    // Save current offset. Pair with Rewind.
    size_t Mark() const;

    // Restore a previously saved offset. Asserts against forward-rewind.
    void Rewind(size_t snapshot);

    // Bytes remaining from current offset to capacity.
    size_t Remaining() const;

    // Carve a sub-arena out of this arena. The child is zeroed.
    // Use parent's alignment unless you have a reason not to.
    Arena Split(size_t split_size, size_t alignment = MASTER_ARENA_ALIGNMENT);

    // Raw bump allocation. Returns aligned pointer, advances offset.
    void* PushAligned(size_t size, size_t alignment);

    // Typed convenience. Allocates count objects worth of bytes, no construction.
    // PODs only. For non-POD types, construct manually after this returns.
    template <typename T>
    T* Push(size_t count = 1, size_t alignment = alignof(T)) {
        assert(count <= SIZE_MAX / sizeof(T));
        size_t totalCount = sizeof(T) * count;
        return static_cast<T*>(PushAligned(totalCount, alignment));
    }
};

// Master arena lifecycle. 
// This is just for MasterArena since others are not really allocated
// as separate memory blocks.
Arena ArenaAllocate(size_t capacity);
void  ArenaFree(Arena* arena);