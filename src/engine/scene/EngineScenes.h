#pragma once
struct Registry;
struct CommandBus;

struct Scene {
  void (*Init)(Registry &reg, CommandBus &bus);
  void (*Update)(Registry &reg, CommandBus &bus, float dt);
  void (*Draw)(Registry &reg, CommandBus &bus);
  void (*Unload)(Registry &reg, CommandBus &bus);
};

