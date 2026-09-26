#pragma once
#include "engine/ecs/SparseSet.h"
#include <stdint.h>

// this is a unique integer counter, 
// so that we can assign unique ids to each component type
struct TypeIdCounter {
    static uint32_t next() {
        static uint32_t counter = 0;
        return counter++;
    }
};
template <typename T>
struct TypeId {
    static uint32_t value() {
        static const uint32_t id = TypeIdCounter::next();
        return id;
    }
};


struct ComponentPool {
    void* set = nullptr;
    uint32_t type_id = 0;

    bool (*remove)(void* set,uint32_t entityID);
    void (*clear)(void* set);
};

// builds a ComponentPool from a SparseSet<T>*
// the ComponentPool is a wrapper around the SparseSet,
// providing type-erased dispatch to the set's data
// using lambda expressions to call the set's methods 
template <typename T>
[[nodiscard]] ComponentPool make_pool(SparseSet<T>* set) {
    return ComponentPool{
      .set = set,
      .type_id = TypeId<T>::value(),
      .remove = [](void* set, uint32_t entityID) {
        return static_cast<SparseSet<T>*>(set)->remove(entityID);
      },
      .clear = [](void* set) {
        static_cast<SparseSet<T>*>(set)->clear();
      }
    };
}