#include "TimeSystem.h"
#include "Platform.h"

void timeSystem_Init(TimeContext *time) {
  time->realDt = 0.0f;
  time->gameDt = 0.0f;
  time->fixedDt = 0.0166667f; // 60 FPS fixed timestep
  time->lastTime = static_cast<double>(Platform::getTicksNS());
  time->accumulator = 0.0f;
  time->timeScale = 1.0f;
}
void timeSystem_Update(TimeContext *time) {
  double current = static_cast<double>(Platform::getTicksNS());
  time->realDt = static_cast<float>(current - time->lastTime);
  time->lastTime = current;

  if (time->realDt > 0.1f)
    time->realDt = 0.1f;

  time->gameDt = time->realDt * time->timeScale;
  time->accumulator += time->gameDt;
}

bool timeSystem_ConsumeFixedStep(TimeContext *time) {
  float fixedDt = time->fixedDt;
  if (time->accumulator >= fixedDt) {
    time->accumulator -= fixedDt;
    return true;
  }
  return false;
}
