#ifndef PROBETEMP_H
#define PROBETEMP_H
#include "common.h"
#include <map>
class ProbeThread: public Thread
{
public:
  DEFINE_INSTANCE(ProbeThread)
  String getJsonProbeTemp();
  double getTemp();
  void run() override;
  
private:
	double tempValue;
  std::map<int, double> Bs;
  double getPredefinedValue(int adc);
};

#endif