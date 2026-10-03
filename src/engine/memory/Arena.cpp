#include "Arena.h"
#include "engine/core/LogSystem.h"
#include <cstdlib>
#include <assert.h>
#include <stdio.h>

// using c++ and C libraries mixed is not good. fix this sometime. choose one and use them.
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/mman.h>
    #include <errno.h>
    #endif

inline size_t align_up(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

// platform-specific memory allocation/freeing
namespace {
    size_t os_getPageSize() {
        static size_t cachedPageSize = 0;
        if (cachedPageSize == 0) {
            #ifdef _WIN32
                SYSTEM_INFO info;
                GetSystemInfo(&info);
                cachedPageSize = info.dwPageSize;
            #else
                const long result = sysconf(_SC_PAGESIZE);
                if (result == -1) {
                    const int error = errno;
                    LogSystem::Critical("Failed to get page size: {}", error);
                    return 0;
                }
                cachedPageSize = static_cast<size_t>(result);
            #endif
        }
        return cachedPageSize;
    }
    void* os_allocate(size_t capacity) {
        #ifdef _WIN32
        // Windows side works differently than POSIX
        // There is no equivalent of MAP_NORESERVE on Windows, so we commit the full capacity upfront.
        // Windows memory diagnostics (Task Manager, etc.) will show this arena's full capacity as committed from startup.
        // This is mostly cosmetic.
        // Proper fix, if this ever matters: MEM_RESERVE only at startup, then grow
        // commit in chunks via os_ensureCommitted() as the bump allocator advances
        // Skipping this issue for now due to using linux as primary platform, and this is learning
        // project at first and this issue is mostly cosmetic.
        void* raw_memory = VirtualAlloc(nullptr, capacity, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (raw_memory == nullptr) {
            const DWORD error = GetLastError();
            LogSystem::Critical("Failed to allocate memory: {}", error);
        }
        return raw_memory;
        #else
        void* raw_memory = mmap(nullptr, capacity, PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
        if (raw_memory == MAP_FAILED) {
            const int error = errno;
            LogSystem::Critical("Failed to allocate memory: {}", error);
            }
            return raw_memory;
        #endif
    }

    bool os_free(void* memory, size_t cap) {
        #ifdef _WIN32
            (void)cap;
            if (!VirtualFree(memory, 0, MEM_RELEASE)) {
                const DWORD error = GetLastError();
                LogSystem::Critical("Failed to free memory: {}", error);
                return false;
            }
        #else
            if (munmap(memory, cap) == -1) {
                const int error = errno;
                LogSystem::Critical("Failed to free memory: {}", error);
                return false;
            }
        #endif
        return true;
    }

    void os_releaseUnusedPages(void* ptr, size_t size) {
        if (size == 0 || ptr == nullptr) return;
        #ifdef _WIN32
            if (!VirtualFree(ptr, size, MEM_DECOMMIT)) {
                const DWORD error = GetLastError();
                LogSystem::Critical("Failed to release unused pages: {}", error);
            }
            if (!VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE)) {
                const DWORD error = GetLastError();
                LogSystem::Critical("Failed to release unused pages: {}", error);
            }
        #else
            if (madvise(ptr, size, MADV_DONTNEED) == -1) {
                const int error = errno;
                LogSystem::Critical("Failed to release unused pages: {}", error);
            }
        #endif
    }
}

Arena ArenaAllocate(size_t capacity) {
    size_t page_size = os_getPageSize();
    
    assert(capacity > 0);
    assert(capacity <= (SIZE_MAX - (page_size - 1)) && "[ARENA] Requested Allocation size too large!");
    size_t aligned_capacity = align_up(capacity, page_size);
    void* raw_memory = os_allocate(aligned_capacity);
    Arena arena = {};
    arena.Bind(raw_memory, aligned_capacity);
    return arena;
}

void ArenaFree(Arena* arena) {
    if (!arena || !(arena->base_ptr)) return;
    os_free(arena->base_ptr, arena->capacity); // we already log inside the function so not doing it again.
    *arena = {};
}

void Arena::Bind(void* memory, size_t cap) {
    assert(memory != nullptr);
    base_ptr = static_cast<uint8_t*>(memory);
    capacity = cap;
    offset = 0;
    highest_offset = 0;
}

void Arena::Clear() {
    size_t page_size = os_getPageSize();
    os_releaseUnusedPages(base_ptr, align_up(highest_offset, page_size));
    offset = 0;
    highest_offset = 0;
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

    // Update highestOffset if necessary
    if (offset > highest_offset)
        highest_offset = offset;
    
    return aligned_memory;
}
