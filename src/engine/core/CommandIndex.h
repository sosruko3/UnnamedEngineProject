#pragma once

#include "CommandBusDefs.h"          
#include "engine/memory/Arena.h"
#include "engine/common/ArenaArray.h"
#include <stdint.h>

// Positions are stored as uint16_t, so a snapshot can reach at most 65536.
constexpr uint32_t kIndexMaxPosition = 65536;

// A range-for friendly view of T elements reached through a list of positions.
// It owns nothing: element i of the view is data[positions[i]].
template <typename T> struct IndexedView {
  const T *data;
  const uint16_t *positions;
  uint32_t count;

  struct Iterator {
    const T *data;
    const uint16_t *position;
    const T &operator*() const { return data[*position]; }
    Iterator &operator++() {
      position++;
      return *this;
    }
    bool operator!=(const Iterator &other) const {
      return position != other.position;
    }
  };

  Iterator begin() const { return {data, positions}; }
  Iterator end() const { return {data, positions + count}; }
};

// Groups a range of commands by domain, and by order inside each domain.
//
// It reads only the small domains[] and orders[] key arrays, never the commands.
// The result is a list of command positions: every domain's commands are one
// contiguous slice, ordered by `order`, and in push order among equal orders.
// Scratch data: rebuilt for every flush.
struct CommandIndex {
  // Positions (absolute, into commands/domains/orders) grouped by domain.
  ArenaArray<uint16_t> sorted;
  // Temporary used while sorting.
  ArenaArray<uint16_t> scratch;

  // Domain d owns sorted[ domainBegin[d] .. domainBegin[d + 1] ).
  uint32_t domainBegin[DomainCount + 1];

  // Allocates both arrays. capacity = the largest range one build can cover.
  [[nodiscard]] bool init(Arena *arena, uint32_t capacity);

  // Sorts the positions [begin, end) by (domain, order, position).
  // domains / orders are indexed by absolute position.
  void build(const Domain *domains, const Order *orders, uint32_t begin,
             uint32_t end);

  uint32_t countOf(Domain domain) const {
    return domainBegin[static_cast<uint32_t>(domain) + 1] - domainBegin[static_cast<uint32_t>(domain)];
  }

  // The commands of one domain, as a range-for view:
  //   for (const Command &cmd : index.viewOf(Domain::Phys, commands.data())) { ... }
  // Needs ArenaArray::data() const.
  template <typename T>
  IndexedView<T> viewOf(Domain domain, const T *data) const {
    return {data, sorted.data() + domainBegin[static_cast<uint32_t>(domain)],
            countOf(domain)};
  }
};
