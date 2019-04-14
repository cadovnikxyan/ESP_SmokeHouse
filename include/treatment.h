#ifndef TREATMENT_H
#define TREATMENT_H

#include <PID_v1.h>
#include "common.h"
#include "powercontrol.h"
#include "dallastemp.h"
#include "probetemp.h"

class HeatTreatmentThread : public Thread
{
  PowerControlThread* powerControlThread;
  DallasTempThread* dallasTempThread;
  ProbeThread* probeThread;
  ThreadController* controller;
  PID* pid;
  time_t start;
  time_t stop;
  bool startingFlag;
 
public:
  DEFINE_INSTANCE(HeatTreatmentThread)
  void startTreatment();
  void stopTreatment();
  String setHeating(bool state);
  String setConvection(bool state);
  String setAirPump(bool state);
  String setWaterPump(bool state);
  String getJsonTemps() const;
  String setMode(const String& mode);
  String getCurrentState() const;
  void setSettings(const String& settings);
  void run() override;
  void setStart( bool flag);
  bool getStartFlag() const;

private:
  void setUpPID();
  void adjustPID();

  double setPoint = 100;
  double aggKp=4, aggKi=0.2, aggKd=1;
  double consKp=1, consKi=0.05, consKd=0.25;
  
};

#endif