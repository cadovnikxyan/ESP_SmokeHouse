#ifndef POWERCONTROL_H
#define POWERCONTROL_H
#include "common.h"

class PowerControlThread : public Thread
{
private:
  uint8_t currentMode;

public:
  DEFINE_INSTANCE(PowerControlThread)
  String setHeating(bool state);
  String setConvection(bool state);
  String setWaterPump(bool state);
  String setAirPump(bool state);
  String setMode(const String& mode);
  uint8_t getCurrentMode() const;
  double* getPowerValueRef(){ return &powerValue; }
  inline void onRelay(int relayPin, int value);
  void run() override;

private:
  double powerValue;

};

#endif