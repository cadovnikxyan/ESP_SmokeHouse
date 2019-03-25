#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Ticker.h>
#include <time.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>
#include "treatment.h"

class WebServerThread : public Thread
{
private:
  BearSSL::ESP8266WebServerSecure* server;
  HeatTreatmentThread* heatTreatmentThread;
  ThreadController* controller;
  MDNSResponder mdns;

public:
  DEFINE_INSTANCE(WebServerThread)
  void handleRoot();
  void handleNotFound();
  void run() override;
};

#endif
