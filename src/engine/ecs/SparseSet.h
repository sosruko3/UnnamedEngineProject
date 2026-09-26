#pragma once
#include "engine/common/ArenaArray.h"

inline constexpr uint32_t SPARSE_INVALID = UINT32_MAX;

template <typename T>
struct SparseSet {
  // sparse: maps entity ID to dense index
  // dense: stores entities in order
  // components: stores component data
  ArenaArray<uint32_t> sparse;
  ArenaArray<uint32_t> dense;
  ArenaArray<T> components;

  // initialize the sparse set with the given capacity
  [[nodiscard]] bool init(Arena* arena, uint32_t capacity) {
    assert(arena != nullptr);
    assert(capacity > 0);
    if (!sparse.init(arena, capacity))
      return false;
    if (!dense.init(arena, capacity))
      return false;
    if (!components.init(arena, capacity))
      return false;

    // sparse needs max capacity from the start
    sparse.resize(capacity);
    // fill with invalid indices to start
    // this is removed due to lazy sparse set lookup
    // sparse.fillAll(SPARSE_INVALID);
    return true;
  }

  // add a component for the given entity ID
  void add(uint32_t entityID, const T& component) {
    assert(entityID < sparse.size());
    assert(!has(entityID) && "component already exists");
    
    sparse[entityID] = dense.size(); // its at the end of dense,thus we ask size.
    
    dense.push(entityID);
    components.push(component);
  }

  // remove the component for the given entity ID
  bool remove(uint32_t entityID) {
    if (!has(entityID)) {
      return false;
    }
    
    
    uint32_t denseIndex = sparse[entityID];
    uint32_t lastDenseIndex = dense.size() - 1;
    uint32_t movedIndex = dense[lastDenseIndex];

    // check if the dense index is valid and the entity ID matches
    if (denseIndex >= dense.size() || dense[denseIndex] != entityID) {
      return false;
    }
    
    // move the last element to the removed index
    dense[denseIndex] = movedIndex;
    components[denseIndex] = components[lastDenseIndex];
    sparse[movedIndex] = denseIndex;
    // invalidate the removed index
    // sparse[entityID] = SPARSE_INVALID; // not needed anymore due to lazy lookup
    // remove the last element
    dense.pop();
    components.pop();
    return true;
  }

  // check if the given entity ID has a component
  [[nodiscard]] bool has(uint32_t entityID) const {
    assert(entityID < sparse.size());
    // Lazy sparse set lookup, check if the dense index is valid and the entity ID matches
    uint32_t denseID = sparse[entityID];
    return denseID < dense.size() && dense[denseID] == entityID;
  }

  // get the component for the given entity ID
  [[nodiscard]] T* get(uint32_t entityID) {
      assert(entityID < sparse.size());
      return has(entityID) ? &components[sparse[entityID]] : nullptr;
  }
  [[nodiscard]] const T* get(uint32_t entityID) const {
      assert(entityID < sparse.size());
      return has(entityID) ? &components[sparse[entityID]] : nullptr;
  }

  // get the number of components
  [[nodiscard]] uint32_t count() const {
      return dense.size(); // same size for dense and components
  }

  [[nodiscard]] uint32_t entity_at(uint32_t dense_idx) const {
      return dense[dense_idx];
  }
  
  // get an iterator to the beginning of the components
  [[nodiscard]] T*       begin()       { return components.begin(); }
  [[nodiscard]] T*       end()         { return components.end(); }
  [[nodiscard]] const T* begin() const { return components.begin(); }
  [[nodiscard]] const T* end()   const { return components.end(); }

  // Wipe all components. Sparse must be reset for future queries.
  // this is O(capacity) right now.
  void clear() {
    dense.resize(0);
    components.resize(0);
    sparse.fillAll(SPARSE_INVALID);
  }
};
