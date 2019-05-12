#ifndef DALLASTEMP_H
#define DALLASTEMP_H
#include "common.h"
#include <OneWire.h>
#include <DallasTemperature.h>

class DallasTempThread : public Thread
{
  OneWire* tempWire;
  DallasTemperature* temp;
  std::vector<float> temps;
  double resultTemp;
  ~DallasTempThread();

public:
  DEFINE_INSTANCE(DallasTempThread)
  String getJsonTemp();
  double* getResultTemp();
  float calculateAverage() const;
  void run() override;
};

#endif