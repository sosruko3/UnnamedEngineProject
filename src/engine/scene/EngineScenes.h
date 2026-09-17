#pragma once
struct EntityRegistry;
struct CommandBus;

struct Scene {
  void (*Init)(EntityRegistry &reg, CommandBus &bus);
  void (*Update)(EntityRegistry &reg, CommandBus &bus, float dt);
  void (*Draw)(EntityRegistry &reg, CommandBus &bus);
  void (*Unload)(EntityRegistry &reg, CommandBus &bus);
};

