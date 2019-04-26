#include "webServer.h"

DECLARATE_INSTANCE(WebServerThread)
WebServerThread::WebServerThread()
   :
     server(new ESP8266WebServer(80)),
     heatTreatmentThread(HeatTreatmentThread::instance()),
     controller(new ThreadController)
{
   if (!MDNS.begin("cadovnik_esp8266")){
      Serial.println("failed to start MDNS");
   }
   MDNS.addService("https", "tcp", 443);
   controller->add(heatTreatmentThread);
   heatTreatmentThread->setInterval(100);
   server->on("/", HTTP_GET, [this]()
   {
      this->handleRoot();
   });

   server->on("/GetTemperatures", HTTP_GET, [this]()
   {
      server->send(200, "application/json", heatTreatmentThread->getJsonTemps());
   });

   server->on("/GetCurrentState", HTTP_GET, [this]()
   {
      server->send(200, "application/json", heatTreatmentThread->getCurrentState());
   });

   server->on("/SetHeating", HTTP_POST, [this]()
   {
      int args = server->args();
      String val = server->arg(0);
      server->send(200, "application/json", heatTreatmentThread->setHeating(val == "true" ? true : false));
   });

    server->on("/SetWaterPump", HTTP_POST, [this]()
   {
      int args = server->args();
      String val;
      for ( int i = 0; i< args; ++i)
      {
         val = server->arg(i);
         if ( val.length() != 0 )
         {
            break;
         }
      }
      server->send(200, "application/json", heatTreatmentThread->setConvection(val == "true" ? true : false));
   });

   server->on("/SetConvection", HTTP_POST, [this]()
   {
      int args = server->args();
      String val;
      for ( int i = 0; i < args; ++i)
      {
         val = server->arg(i);
         if ( val.length() != 0 )
         {
            break;
         }
      }
      server->send(200, "application/json", heatTreatmentThread->setConvection(val == "true" ? true : false));
   });

   server->on("/StartHeatTreatment", HTTP_POST, [this]()
   {
      String respose = "{started : false}";
      if ( controller->shouldRun() )
      {
         Serial.println("Start Treatment");
         heatTreatmentThread->setStart(true);
         controller->add(heatTreatmentThread);
         respose = "{started : true}";
      }
      server->send(200, "application/json", respose);
   });

   server->onNotFound([this]()
   {
      handleNotFound();
   });
   server->begin();
}

void WebServerThread::handleRoot()
{
   server->send(200, "text/html", "Hello!!!\nThis device doesn`t have a http gui, use android application : HomeSmartGardner");
}

void WebServerThread::handleNotFound()
{
   String message = "File Not Found\n\n";
   message += "URI: ";
   message += server->uri();
   message += "\nMethod: ";
   message += (server->method() == HTTP_GET)?"GET":"POST";
   message += "\nArguments: ";
   message += server->args();
   message += "\n";
   for (uint8_t i=0; i<server->args(); i++)
   {
      message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
   }
   server->send(404, "text/plain", message);
}

void WebServerThread::run()
{
   server->handleClient();
  if (! MDNS.update()) {
     Serial.println("failed to update MDNS!");
  }
   controller->run();
   runned();
}