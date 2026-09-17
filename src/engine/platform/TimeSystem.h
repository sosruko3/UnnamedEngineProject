#pragma once

struct TimeContext {
  float realDt;
  float gameDt;
  float fixedDt;
  double lastTime;
  float accumulator;
  float timeScale;
};

void timeSystem_Init(TimeContext *time);
void timeSystem_Update(TimeContext *time);
bool timeSystem_ConsumeFixedStep(TimeContext *time);
