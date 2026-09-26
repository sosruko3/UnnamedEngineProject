#pragma once

#include "engine/common/EngineTypes.h"
#include "engine/ecs/ComponentPool.h"
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/SparseSet.h"

#include <memory>
#include <stddef.h>
#include <stdint.h>
#include <type_traits>

struct Registry {
    static constexpr uint32_t MAX_COMPONENTS = 64;

    // Owns the entity manager and registered component pools. The arena owns
    // the storage referenced by these objects and arrays.
    EntityManager entityManager{};
    Arena* arena_ = nullptr;
    ComponentPool componentPools[MAX_COMPONENTS]{};
    uint32_t componentCount = 0;

    [[nodiscard]] bool init(Arena* arena);
    void shutdown();

    template <typename T>
    [[nodiscard]] bool registerComponent();

    template <typename T>
    [[nodiscard]] SparseSet<T>* componentPool();

    template <typename T>
    [[nodiscard]] const SparseSet<T>* componentPool() const;

    [[nodiscard]] Entity create();
    void destroy(Entity entity);
    [[nodiscard]] bool isValid(Entity entity) const;

    template <typename T>
    [[nodiscard]] bool add(Entity entity, const T& component);

    template <typename T>
    [[nodiscard]] bool remove(Entity entity);

    template <typename T>
    [[nodiscard]] T* get(Entity entity);

    template <typename T>
    [[nodiscard]] const T* get(Entity entity) const;

    template <typename T>
    [[nodiscard]] bool has(Entity entity) const;
};

template <typename T>
bool Registry::registerComponent() {
    static_assert(std::is_trivially_copyable_v<T>,
                  "Arena-backed components must be trivially copyable");
    static_assert(std::is_trivially_destructible_v<T>,
                  "Arena-backed components must be trivially destructible");

    if (!arena_)
        return false;

    // check if component id is valid and not already registered
    const uint32_t typeId = TypeId<T>::value();
    if (typeId >= MAX_COMPONENTS)
        return false;

    ComponentPool& pool = componentPools[typeId];
    // if component is already registered, return true if the type id matches
    // otherwise, return false
    if (pool.set)
        return pool.type_id == typeId;


    // allocate memory for the sparse set and its components
    // if allocation fails, rewound the arena and return false
    const size_t arenaMark = arena_->Mark();
    auto* set = std::construct_at(arena_->Push<SparseSet<T>>());
    
    if (!set) {
        arena_->Rewind(arenaMark);
        return false;
    }
    if (!set->init(arena_, MAX_ENTITIES)) {
        arena_->Rewind(arenaMark);
        return false;
    }

    pool = make_pool(set);
    ++componentCount;
    return true;
}

template <typename T>
SparseSet<T>* Registry::componentPool() {
    if (!arena_)
        return nullptr;

    const uint32_t typeId = TypeId<T>::value();
    if (typeId >= MAX_COMPONENTS)
        return nullptr;

    ComponentPool& pool = componentPools[typeId];
    if (!pool.set || pool.type_id != typeId)
        return nullptr;

    return static_cast<SparseSet<T>*>(pool.set);
}

template <typename T>
const SparseSet<T>* Registry::componentPool() const {
    if (!arena_)
        return nullptr;

    const uint32_t typeId = TypeId<T>::value();
    if (typeId >= MAX_COMPONENTS)
        return nullptr;

    const ComponentPool& pool = componentPools[typeId];
    if (!pool.set || pool.type_id != typeId)
        return nullptr;

    return static_cast<const SparseSet<T>*>(pool.set);
}

template <typename T>
bool Registry::add(Entity entity, const T& component) {
    if (!isValid(entity))
        return false;
    if (has<T>(entity))
        return false;

    SparseSet<T>* pool = componentPool<T>();
    if (!pool)
        return false;

    pool->add(entity.index(), component);
    return true;
}

template <typename T>
bool Registry::remove(Entity entity) {
    if (!isValid(entity))
        return false;

    SparseSet<T>* pool = componentPool<T>();
    return pool && pool->remove(entity.index());
}

template <typename T>
T* Registry::get(Entity entity) {
    if (!isValid(entity))
        return nullptr;

    SparseSet<T>* pool = componentPool<T>();
    return pool ? pool->get(entity.index()) : nullptr;
}

template <typename T>
const T* Registry::get(Entity entity) const {
    if (!isValid(entity))
        return nullptr;

    const SparseSet<T>* pool = componentPool<T>();
    return pool ? pool->get(entity.index()) : nullptr;
}

template <typename T>
bool Registry::has(Entity entity) const {
    if (!isValid(entity))
        return false;

    const SparseSet<T>* pool = componentPool<T>();
    return pool && pool->has(entity.index());
}
