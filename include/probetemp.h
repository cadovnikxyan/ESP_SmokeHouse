#ifndef PROBETEMP_H
#define PROBETEMP_H
#include "common.h"

class ProbeThread: public Thread
{
public:
  DEFINE_INSTANCE(ProbeThread)
  String getJsonProbeTemp();
  double getTemp();
  void run() override;
  
private:
	double tempValue;
};

#endif