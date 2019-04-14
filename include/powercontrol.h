#ifndef POWERCONTROL_H
#define POWERCONTROL_H
#include "common.h"

class PowerControlThread : public Thread
{
private:
  uint8_t currentMode;

public:
  DEFINE_INSTANCE(PowerControlThread)
  void setHeating();
  void setConvection(bool state);
  void setWaterPump(bool state);
  void setAirPump(bool state);
  double* getPowerValuePtr(){ return &powerValue; }
  inline void onRelay(int relayPin, int value);
  void run() override;

private:
  double powerValue;

};

#endif