#pragma once

#include <SDL3/SDL_pixels.h>
#include <stddef.h>
#include <stdint.h>

// Entity
constexpr uint32_t ENTITY_INDEX_BITS = 18;
constexpr uint32_t ENTITY_INDEX_MASK = (1u << ENTITY_INDEX_BITS) - 1;
constexpr uint32_t MAX_ENTITIES       = 1u << ENTITY_INDEX_BITS;
constexpr uint32_t GENERATION_BITS = 32 - ENTITY_INDEX_BITS;
constexpr uint32_t GENERATION_MASK = (1u << GENERATION_BITS) - 1;

static_assert(ENTITY_INDEX_BITS + GENERATION_BITS == 32, "bits must sum to 32");
static_assert(MAX_ENTITIES == (1u << ENTITY_INDEX_BITS), "ENTITY_MAX must be 2^bits");

struct Entity {
    uint32_t id = 0;
    uint32_t index() const      { return id & ENTITY_INDEX_MASK; }
    uint32_t generation() const { return id >> ENTITY_INDEX_BITS; }
    bool operator==(const Entity &other) const { return id == other.id; }
    bool operator!=(const Entity &other) const { return id != other.id; }
};



enum AudioSourceID : uint16_t;
enum AudioGroupID : uint8_t;
enum AudioUsageType : uint8_t;

struct AudioID {
  uint16_t index;
  uint16_t gen;
};



struct creVec2 {
  float x;
  float y;

  // OPERATOR OVERLOADS

  inline creVec2 operator+(const creVec2 &other) const {
    return {x + other.x, y + other.y};
  }
  inline creVec2 operator-(const creVec2 &other) const {
    return {x - other.x, y - other.y};
  }
  inline creVec2 operator*(const creVec2 &other) const {
    return {x * other.x, y * other.y};
  }
  inline creVec2 operator*(float scalar) const {
    return {x * scalar, y * scalar};
  }
};

struct creColor {
  uint8_t r; // red
  uint8_t g; // green
  uint8_t b; // blue
  uint8_t a; // alpha
};

struct creRectangle {
  float x;      // Rectangle top-left corner position x
  float y;      // Rectangle top-left corner position y
  float width;  // Rectangle width
  float height; // Rectangle height
};
