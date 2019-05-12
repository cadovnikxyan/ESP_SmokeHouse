#include "dallastemp.h"


DECLARATE_INSTANCE(DallasTempThread)
DallasTempThread::DallasTempThread()
{
   tempWire = new OneWire(TEMP_PIN);
   temp = new DallasTemperature(tempWire);
   temp->begin();
}

DallasTempThread::~DallasTempThread()
{
   delete temp;
   delete tempWire;
}

String DallasTempThread::getJsonTemp()
{
   temp->begin();
   temp->requestTemperatures();
   int count = temp->getDeviceCount();
   temps.clear();
   StaticJsonDocument<200> root;
   for ( int i = 0; i < count ; ++i)
   {
      float t = temp->getTempCByIndex(i);
      temps.push_back(t);
      root[String("temp") + String(i)] = t;
   }
   getResultTemp();
   __globalState__.currentOutTemp = resultTemp;
   Serial.print("Device count: ");
   Serial.println(count);
   String result;
   serializeJson(root, result);
   return result;
}

float DallasTempThread::calculateAverage() const
{
   float tempSum = 0;
   for ( auto t : temps)
      tempSum += t;
   if ( temps.empty() )
      return tempSum;

   return tempSum / temps.size();
}

double* DallasTempThread::getResultTemp()
{
  float r = calculateAverage();
  resultTemp = static_cast<double>(r);
  return &resultTemp;
}

void DallasTempThread::run()
{
   temp->begin();
   temp->requestTemperatures();
   int count = temp->getDeviceCount();
   temps.clear();
   for ( int i = 0; i < count ; ++i)
      temps.push_back(temp->getTempCByIndex(i));

    getResultTemp();
   __globalState__.currentOutTemp = resultTemp;
   Serial.print("Out temp");
   Serial.println(__globalState__.currentOutTemp);
   runned();
}

