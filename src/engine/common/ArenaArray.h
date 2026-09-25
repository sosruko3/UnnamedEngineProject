#pragma once
#include <stdint.h>
#include <assert.h>
#include "engine/memory/Arena.h" // had to add this due to how templates work

// This data structure is for POD types only, not for classes with constructors/destructors.
// Might have unintended behavior with non-POD types.
// [DOIT]Add extra guardrails for release builds.
// removed reset function, resize(0) is sufficent for such thing.

template <typename T>
struct ArenaArray {
  T* data_ = nullptr; 
  uint32_t size_ = 0;
  uint32_t cap_ = 0;

  bool init(Arena* arena, uint32_t capacity) { 
    assert(data_ == nullptr && "this array is already initialized");
    if (!arena || capacity == 0)
      return false;
    data_ = arena->Push<T>(capacity);
    size_ = 0;
    cap_ = capacity;
    return true;
  }
  
  // Returns a pointer to the data, useful for some cases.
  T* data() { return data_; }
  const T* data() const { return data_; }

  // Pushes an item onto the end of the array.
  void push(const T& item) { 
    assert(size_ < cap_);
    data_[size_++] = item;
  }
  
  // Pops the last item off the array.
  void pop() {
    assert(size_ > 0);
    size_--;
  }
  
  // Fills the array with the given item.
  void fill(const T& item) {
    for (uint32_t i = 0; i < size_; i++)
      data_[i] = item;
  }
  
  // Fills the ENTIRE array with the given item.
  void fillAll(const T& item) {
    for (uint32_t i = 0; i < cap_; i++)
      data_[i] = item;
  }
  
  // Resizes the array to the given size.
  void resize(uint32_t newSize) { 
    assert(newSize <= cap_);
    size_ = newSize;
  }

  // these return the capacity, size, and whether the array is empty or full.
  uint32_t cap() const { return cap_; }
  uint32_t size() const { return size_; }
  bool     empty()    const { return size_ == 0; }
  bool     full()     const { return size_ == cap_; }

  // operators[]
  T&       operator[](uint32_t i)       { assert(i < size_); return data_[i]; }
  const T& operator[](uint32_t i) const { assert(i < size_); return data_[i]; }
  
  // iterators
  T*       begin()       { return data_; }
  T*       end()         { return data_ + size_; }
  const T* begin() const { return data_; }
  const T* end()   const { return data_ + size_; }
  
};