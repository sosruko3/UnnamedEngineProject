#include "Registry.h"
#include "engine/core/LogSystem.h"


bool Registry::init(Arena* arena) {
    if (!arena)
        return false;


    for (uint32_t i = 0; i < MAX_COMPONENTS; i++) {
        componentPools[i] = {};
    }
    componentCount = 0;

    if (!entityManager.init(arena))
        return false;

    arena_ = arena;
    return true;
}

Registry* Registry::createRegistry(Arena* arena) {
    if (arena == nullptr) return nullptr;
    // Allocate memory and initialize the Registry.
    auto* storage = arena->Push<Registry>();
    if (storage == nullptr) return nullptr;
    auto* reg = ::new (storage) Registry{};
    // Initialize the Registry buffer and variables.
    if (!reg->init(arena)) {
        reg->~Registry();
        LogSystem::Error("Failed to initialize Registry");
        return nullptr;
    }
    return reg;
}

void Registry::shutdown() {
    entityManager.shutdown();

    for (uint32_t i = 0; i < MAX_COMPONENTS; i++) {
        componentPools[i] = {};
    }
    componentCount = 0;
    arena_ = nullptr;
}

Entity Registry::create() {
    if (!arena_)
        return Entity{};

    return entityManager.create();
}

void Registry::destroy(Entity entity) {
    if (!isValid(entity))
        return;

    const uint32_t entityIndex = entity.index();
    for (uint32_t i = 0; i < MAX_COMPONENTS; i++) {
        if (componentPools[i].set == nullptr)
            continue;
        // check this part again!
        componentPools[i].remove(componentPools[i].set, entityIndex);
    }

    entityManager.destroy(entity);
}

bool Registry::isValid(Entity entity) const {
  if (!arena_)
    return false;
  return entityManager.isValid(entity);
}
