#include "TimeSystem.h"
#include "Platform.h"

void TimeSystem::init() {
  realDt = 0.0f;
  gameDt = 0.0f;
  fixedDt = 0.0166667f; // 60 FPS fixed timestep
  lastTime = static_cast<double>(Platform::getTicksNS());
  accumulator = 0.0f;
  timeScale = 1.0f;
}
void TimeSystem::update() {
  double current = static_cast<double>(Platform::getTicksNS());
  realDt = static_cast<float>(current - lastTime);
  lastTime = current;

  if (realDt > 0.1f)
    realDt = 0.1f;

  gameDt = realDt * timeScale;
  accumulator += gameDt;
}

bool TimeSystem::consumeFixedStep() {
  float fixedDeltaTime = this->fixedDt;
  if (accumulator >= fixedDeltaTime) {
    accumulator -= fixedDeltaTime;
    return true;
  }
  return false;
}
