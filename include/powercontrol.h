#ifndef POWERCONTROL_H
#define POWERCONTROL_H
#include "common.h"
#include <Ticker.h>

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
  void setIgnition(bool state);

  double* getPowerValuePtr(){ return &powerValue; }
  inline void onRelay(int relayPin, int value);
  void run() override;

private:
  double powerValue;
  Ticker* disableAirPump;
  Ticker* disableWaterPump;
  Ticker* disableIgnitionModule;

};

#endif