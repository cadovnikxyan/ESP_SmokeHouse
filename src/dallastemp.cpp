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
   temp->requestTemperatures();
   tempValue1 = temp->getTempCByIndex(0);
   tempValue2 = temp->getTempCByIndex(1);
   tempValue3 = temp->getTempCByIndex(2);
   __globalState__.currentOutTemp = *getResultTemp();
   Serial.print("Device count: ");
   Serial.println(temp->getDeviceCount());
   StaticJsonDocument<200> jsonBuffer;
   JsonObject root = jsonBuffer.to<JsonObject>();
   root["temp1"] = tempValue1;
   root["temp2"] = tempValue2;
   root["temp3"] = tempValue3;
   String result;
   serializeJson(root, result);
   return result;
}

float DallasTempThread::calculateAverage() const
{
   return (tempValue1 + tempValue2 + tempValue3) / 3;
}

double* DallasTempThread::getResultTemp()
{
  float r = calculateAverage();
  resultTemp = static_cast<double>(r);
  return &resultTemp;
}

void DallasTempThread::run()
{
   temp->requestTemperatures();
   tempValue1 = temp->getTempCByIndex(0);
   tempValue2 = temp->getTempCByIndex(1);
   tempValue3 = temp->getTempCByIndex(2);
   __globalState__.currentOutTemp = *getResultTemp();
   Serial.print("Out temp");
   Serial.println(__globalState__.currentOutTemp);
   runned();
}

