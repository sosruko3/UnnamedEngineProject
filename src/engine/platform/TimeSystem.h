#pragma once

struct TimeSystem {
public:
  void init();
  void update();
  bool consumeFixedStep();
  float realDt;
  float gameDt;
  float fixedDt;
  float timeScale;
private:
  double lastTime;
  float accumulator;
};