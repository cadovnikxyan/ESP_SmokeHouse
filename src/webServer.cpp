#include "webServer.h"

DECLARATE_INSTANCE(WebServerThread)
WebServerThread::WebServerThread()
   :
     server(new ESP8266WebServer(80)),
     heatTreatmentThread(HeatTreatmentThread::instance()),
     controller(new ThreadController)
{
   // if (!mdns.begin("cadovnik_esp8266")){
   //    Serial.println("failed to start MDNS");
   // }
   // mdns.addService("http", "tcp", 80);
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

   server->on("/SetState", HTTP_POST, [this]()
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
      server->send(200,"application/json", heatTreatmentThread->setState(val));
      if ( true == heatTreatmentThread->getStartFlag() && controller->shouldRun() )
         controller->add(heatTreatmentThread);
   });

   server->on("/DoCalibrateProbe", HTTP_POST, [this]()
   {
      heatTreatmentThread->doCalibrateProbe();
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
//   if (!mdns.update()) {
//      Serial.println("failed to update MDNS!");
//   }
   controller->run();
   runned();
}