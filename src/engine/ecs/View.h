#pragma once
#include <tuple>
#include <engine/ecs/Registry.h>
#include <engine/ecs/SparseSet.h>


template <typename... Components>
struct View {
  static_assert(sizeof...(Components) > 0, "View needs at least one component");
  // Holds one pool pointer per component type. std::tuple lets a single
  // View<A, B, ...> template cover any number of components, instead of
  // hand-writing View2<A,B>, View3<A,B,C>, etc.
  std::tuple<SparseSet<Components>*...> pools;
  EntityManager* entityManager = nullptr;
  
  // initialize
  [[nodiscard]] bool init(Registry& registry) {
    // Calls registry->componentPool<T>() once per component type and
    // packs the resulting pointers into the tuple.
    pools = std::make_tuple(registry.componentPool<Components>()...);
    entityManager = &registry.entityManager;
    // && ... evaluates to true if all components have a valid pool.(Fold expression)
    // std::get<Type>(tuple) returns a reference to the field of type Type in the tuple.
    return (std::get<SparseSet<Components>*>(pools) && ...);
  }

  // Fetch one pool by component type, as a reference.
  // The pointers were checked non-null in init(), so dereferencing is safe.
  template <typename T>
  SparseSet<T>& poolRef() const { 
    return *std::get<SparseSet<T>*>(pools);
  }

  // Func is deducted here, so the caller can pass a lambda or function pointer.
  // const qualified to avoid modifying the view's state.
  template <typename Func>
  void iterate(Func&& func) const {
    assert(entityManager != nullptr);
    assert((std::get<SparseSet<Components>*>(pools) && ...));
    
    // Get the count and dense data for each component.
    const uint32_t  counts[] = { poolRef<Components>().count()... };
    const uint32_t* denses[] = { poolRef<Components>().dense.data()... };
    
    // Pick the pool with the fewest entries.
    uint32_t smallest = 0;
    for (uint32_t i = 1; i < sizeof...(Components); i++) {
        if (counts[i] < counts[smallest])
            smallest = i;
    }

    const uint32_t  smallestCount    = counts[smallest];
    const uint32_t* smallestEntities = denses[smallest];

    // Walk the smallest pool's entity indices.
    for (uint32_t i = 0; i < smallestCount; i++) {
        const uint32_t entityID = smallestEntities[i];

        // Skip entities missing any component.
        // Expands to: poolRef<A>().has(entityID) && poolRef<B>().has(entityID) && ...
        if (!(poolRef<Components>().has(entityID) && ...)) 
            continue;

        // Call the user's function: the entity, then one reference
        // per component, in template order.
        func(entityManager->getEntity(entityID),
             *poolRef<Components>().get(entityID)...);
    }    
  }
};



