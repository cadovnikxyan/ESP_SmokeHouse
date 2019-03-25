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
#define RELAY_PIN_1   D1 // TEH_TOP
#define RELAY_PIN_2   D2 // TEH_BOTTOM
#define RELAY_PIN_3   D3 // WATER_PUMP
#define RELAY_PIN_4   D4 // AIR_PUMP
#define RELAY_PIN_5   D5 // CONVECTION_COOLER_1
#define RELAY_PIN_6   D6 // CONVECTION_COOLER_2
#define RELAY_COUNT   6

#define TEH_TOP             RELAY_PIN_1
#define TEH_BOTTOM          RELAY_PIN_2
#define WATER_PUMP          RELAY_PIN_3
#define AIR_PUMP            RELAY_PIN_4
#define CONVECTION_COOLER_1 RELAY_PIN_5
#define CONVECTION_COOLER_2 RELAY_PIN_6

#define SO            D7 //NOT_USED
#define CS            D8 //NOT_USED

#define B 3899
#define SERIAL_R 10000
#define THERMISTOR_R 200000
#define NOMINAL_T 25

#define DRYING_DONE_TEMP 40
#define FRYING_DONE_TEMP 60
#define FULL_DONE_TEMP 71

#define MANUAL_MODE   0x01
#define AUTO_MODE     0x02
#define NO_HEATING    0x03
#define SMOKING_MODE  0x04


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
  float calculateAverage() const;
  void run() override;
};

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

  inline void onRelay(int relayPin, int state);
  void run() override;
private:

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
  
  
};

#endif