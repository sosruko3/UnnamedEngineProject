#include "CommandIndex.h"
#include <assert.h>

constexpr uint32_t kOrderValues = 256; // Order is a uint8_t

// Stable counting sort of positions by key[position].
//   in / out : n positions (must be different arrays)
//   keyBegin : keyCount + 1 entries; keyBegin[k] receives the first output slot
//              of key k, and keyBegin[keyCount] == n.
// Three steps: count per key, turn the counts into start offsets (prefix sum),
// then place every position in input order. Placing in input order is what
// keeps the sort stable.
template <typename Key>
static void StableCountingSort(const uint16_t *in, uint16_t *out, uint32_t n,
                               const Key *key, uint32_t keyCount,
                               uint32_t *keyBegin) {
  assert(keyCount <= kOrderValues);
  uint32_t nextFree[kOrderValues];

  // 1. count how many positions have each key
  for (uint32_t k = 0; k <= keyCount; k++) {
    keyBegin[k] = 0;
  }
  for (uint32_t i = 0; i < n; i++) {
    const uint32_t k = static_cast<uint32_t>(key[in[i]]);
    assert(k < keyCount);
    keyBegin[k]++;
  }

  // 2. counts -> start offsets: each key gets a region of exactly its size
  uint32_t running = 0;
  for (uint32_t k = 0; k < keyCount; k++) {
    const uint32_t countOfKey = keyBegin[k];
    keyBegin[k] = running;
    nextFree[k] = running;
    running += countOfKey;
  }
  keyBegin[keyCount] = running; // == n

  // 3. place every position into its key's region, in input order
  for (uint32_t i = 0; i < n; i++) {
    const uint32_t k = static_cast<uint32_t>(key[in[i]]);
    out[nextFree[k]] = in[i];
    nextFree[k]++;
  }
}

bool CommandIndex::init(Arena *arena, uint32_t capacity) {
  assert(arena != nullptr);
  assert(capacity > 0 && capacity <= kIndexMaxPosition);

  if (!sorted.init(arena, capacity))
    return false;
  if (!scratch.init(arena, capacity))
    return false;

  for (uint32_t d = 0; d <= DomainCount; d++) {
    domainBegin[d] = 0;
  }
  return true;
}

void CommandIndex::build(const Domain *domains, const Order *orders,
                         uint32_t begin, uint32_t end) {
  assert(domains != nullptr && orders != nullptr);
  assert(begin <= end);
  assert(end <= kIndexMaxPosition);

  const uint32_t n = end - begin;
  assert(n <= sorted.cap());

  sorted.resize(n);
  scratch.resize(n);
  uint16_t *sortedData = sorted.data();
  uint16_t *scratchData = scratch.data();

  // The input of the first sort: the positions begin .. end-1 in push order.
  for (uint32_t i = 0; i < n; i++) {
    sortedData[i] = static_cast<uint16_t>(begin + i);
  }

  // Sort by the minor key first, then by the major key. Each pass is stable,
  // so the final order is (domain, order, push order).
  uint32_t orderBegin[kOrderValues + 1]; // not needed afterwards
  StableCountingSort(sortedData, scratchData, n, orders, kOrderValues,
                     orderBegin);
  StableCountingSort(scratchData, sortedData, n, domains, DomainCount,
                     domainBegin); // also fills the per-domain slices
}