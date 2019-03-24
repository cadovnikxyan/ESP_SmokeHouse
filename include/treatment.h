#ifndef TREATMENT_H
#define TREATMENT_H

#include <ArduinoJson.h>
#include "Thread.h"
#include "ThreadController.h"
#include <Ticker.h>
#include <time.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DEFINE_INSTANCE(Class) private:\
                               Class();\
                               public: \
                               static Class* instance();
                               
#define DECLARATE_INSTANCE(Class)  static Class* static##Class##Ptr = nullptr;\
                                   Class* Class::instance(){\
                                       if ( static##Class##Ptr == nullptr )\
                                          {\
                                            static##Class##Ptr = new Class();\
                                          }\
                                       return static##Class##Ptr;\
                                    }    

#define THERMISTORPIN A0 
#define TEMP_PIN      D0
#define RELAY_PIN_1   D1
#define RELAY_PIN_2   D2
#define RELAY_PIN_3   D3 
#define RELAY_PIN_4   D4
#define RELAY_PIN_5   D5
#define RELAY_PIN_6   D6
#define RELAY_COUNT   6

#define SO            D7 //NOT_USED
#define CS            D8 //NOT_USED

#define B 3899
#define SERIAL_R 10000
#define THERMISTOR_R 200000
#define NOMINAL_T 25

#define DRYING_DONE_TEMP 40
#define FRYING_DONE_TEMP 60
#define FULL_DONE_TEMP 71


class ProbeThread: public Thread
{
	double tempValue;
public:
  DEFINE_INSTANCE(ProbeThread)
  String getJsonProbeTemp();
  double getTemp();
  void run() override;

};

class DallasTempThread : public Thread
{
  OneWire* tempWire;
  DallasTemperature* temp;
  float tempValue1;
  float tempValue2;
  float tempValue3;
  ~DallasTempThread();

public:
  DEFINE_INSTANCE(DallasTempThread)
  String getJsonTemp();
  void run() override;
};

class PowerControlThread : public Thread
{
public:
  DEFINE_INSTANCE(PowerControlThread)
  void onRelay(int relayPin, int state);
  String setHeatingOn();
  String setHeatingOff();
  String setConvectionOn();
  String setConvectionOff();
  void run() override;
};

class HeatTreatmentThread : public Thread
{
  PowerControlThread* powerControlThread;
  DallasTempThread* dallasTempThread;
  ProbeThread* probeThread;
  ThreadController* controller;
  time_t start;
  time_t stop;
  bool startingFlag;

public:
  DEFINE_INSTANCE(HeatTreatmentThread)
  void startTreatment();
  void stopTreatment();
  String setHeatingOn();
  String setHeatingOff();
  String setConvectionOn();
  String setConvectionOff();
  String getJsonTemp();
  String getJsonProbeTemp();
  void run() override;
  void setStart( bool flag);
  bool getStartFlag() const;
  
};

#endif