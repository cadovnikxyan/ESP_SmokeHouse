#include "webServer.h"


static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIID2DCCAsCgAwIBAgIJAMJ05qYk2EWCMA0GCSqGSIb3DQEBCwUAMIGAMQswCQYD
VQQGEwJSVTEPMA0GA1UECAwGTW9zY293MRMwEQYDVQQHDApaZWxlbm9ncmFkMQww
CgYDVQQKDANIU0cxGjAYBgNVBAMMEUFsZXhleSBIb3RjaGVua292MSEwHwYJKoZI
hvcNAQkBFhJob3RjaGVua292QGxpc3QucnUwHhcNMTkwMzA1MTM0NTIyWhcNMjAw
MzA0MTM0NTIyWjCBgDELMAkGA1UEBhMCUlUxDzANBgNVBAgMBk1vc2NvdzETMBEG
A1UEBwwKWmVsZW5vZ3JhZDEMMAoGA1UECgwDSFNHMRowGAYDVQQDDBFBbGV4ZXkg
SG90Y2hlbmtvdjEhMB8GCSqGSIb3DQEJARYSaG90Y2hlbmtvdkBsaXN0LnJ1MIIB
IjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvZPjlqkMn3ykl3Sc7q8eGcTs
Tvu1FOe+6gk/YkKrqdbozo+r2MA/XSq88NKwz0tVrJOit7SpU46bgeLwcTIWABPL
SSLqSTc83ttC636s4lmgGIggCB99ymmWnZCAI+gDalIZlvaKrHTBZzaKQw6CQKQf
R4hLLa3g+N+11H2prrq+s1Ny74+1+uSnlPRN9gdyz8qcEtBNvTQfLbNSUYFiFhLl
5qk8nTKAA+5tQsa6eOU2SkfcYjulF9MbGe79OE61rl7nlwCcaMePv7mrUDLExZ55
h08TSaCuJvx44cTbrGB1nqMhtHY7TuzYSTTjjzGwfonC+JF1AeAK5WK6L66TQQID
AQABo1MwUTAdBgNVHQ4EFgQUojufPz1vVvnfLFkDiGby5sNGzd4wHwYDVR0jBBgw
FoAUojufPz1vVvnfLFkDiGby5sNGzd4wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG
9w0BAQsFAAOCAQEAi6sWXqdWFBfftRDjViU76MB85DSqgrDnmEbo7HCsbvb0boPo
3cFa2HuOekNXBgtsdPcDJjqtmwah9MtFvyX271ttT4EgklHcwj03nWxvwAnuiuaX
mxKSj1WtkJPMD8WOZUj3Ef+6SSDgR5tv/FNcR8NftIVCpaaL+M2lRqdPJhkWiSen
U053vrDnV9eS6vsWFH3n+cUura2NwbpMDH/JuA/eSDMXjJ7sjCxEUc/Vz3/fkPKu
2BPKaEtzt4i6HtGqkBoqGpHZ/hiqQQw106j/ej3ro3K/ooySNjealwtxtZyAgJPn
WQ4knKHg4Pv0qW/ya0C3UL+wajEDddk9BLDHEA==
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC9k+OWqQyffKSX
dJzurx4ZxOxO+7UU577qCT9iQqup1ujOj6vYwD9dKrzw0rDPS1Wsk6K3tKlTjpuB
4vBxMhYAE8tJIupJNzze20LrfqziWaAYiCAIH33KaZadkIAj6ANqUhmW9oqsdMFn
NopDDoJApB9HiEstreD437XUfamuur6zU3Lvj7X65KeU9E32B3LPypwS0E29NB8t
s1JRgWIWEuXmqTydMoAD7m1Cxrp45TZKR9xiO6UX0xsZ7v04TrWuXueXAJxox4+/
uatQMsTFnnmHTxNJoK4m/HjhxNusYHWeoyG0djtO7NhJNOOPMbB+icL4kXUB4Arl
YrovrpNBAgMBAAECggEAVysG/KFiUnsHWzSdbApNNuAbH/4QtzoPyKDT6iRptkBA
76mUBKfw+q/KDJlz/vzOuflwWjPRn7/0tPZaRFIZV5PCZzCMtXmWnYoH7Sxj5ff7
6wu8Xr/MISQHww9MPDZUS7DQX1+hR8avnvuu+0JSldJhJ4z9tFU9GnISs0/Mk7Ay
Xx0HozohsFSq9qU2BNKihwqz/mkyBpQpvNU6KyAUdDBeVOTU8KIz3m0vra2+3uip
dt+GQdzIeltozzYSAWsKuCFYWL47qkA8jRA8cKpJ/Mel2+EkYO8Mi0i04GNLstx3
SVc2Nqe8tVjIpNrZFEUsshoB0FgMPAhRPhY0x8rOgQKBgQD2YHHpMnRN2Qy1sbGR
wXDjbTrAPHdkUXAqLRo0bm1PKhH0KzL62Ccg00VJAxdv+wfrKWHiAU26h8ikliBr
dwhPDcwdzi472KBmdcqJxRfKA7s8C1GkAusRcvRX2fvX0fBiihne9JutWIjLarFn
HXM9TuAlPUMrrwbCJiR2UbFrmQKBgQDE+4Cmd8lib7WPwT5hQOagSye7AGxhKChV
+ira6Amgzz+RXer3y+HKiE6hf1ZJ5DQuXfjG+EHhvBlR6VYYLGA+INYRUJts8fyX
s2f5QCcjrdd1y7gcMNAsUszHZRAnmbxLPILgQUo00dS7sLpQ3tbs6gueoRK+7t7P
9Mksn6/t6QKBgQCpM3MHg+77x6RC49XfE8pDZjCC9QPJkWRGktL8Txoi0/Ggjkpk
my3EDVP34HeoH+5AQhtG6z5GbBoPbkq1gvnh0MftcDuwPgVqWFrQYtsDkax0gBGB
Yb+uLBRCgSHEBhngkpdR1wtS8QqBKLTM7BzIbfLhoEERLxKlIyzHVTNTgQKBgQC9
O9uwkFz/2s8yRfPWXBHypaCKbnKvt3NgfU8UM1ukh9TKos3qtYu5FM9tKDZOEBPt
A0vOYYpxAFHWtfGGkYL3zHW59pUJyULevXRCJ9Yx2XymZkn9kws5uWWOnFEzqeYQ
5RXZt/9A1N+oW7PxotIYkRMOljC7CiE06DUnUFUjmQKBgB3LKcSqtAoOF9IzLHDZ
X8r84W9lC1ZRbQ+zz7LWHutKNc/agtqqMaPin0r4Qkf2NSHPrzxwOoih51Kcamld
3/fhx/dAPJhCbHycCdzGod9tjKKjv+C+NWI/8uL26NzdYIaXcwjNKri7TcLnirga
NhmksZwnF4D9Z0tyW6EiQBIV
-----END PRIVATE KEY-----
)EOF";

DECLARATE_INSTANCE(WebServerThread)
WebServerThread::WebServerThread()
   :
     server(new BearSSL::ESP8266WebServerSecure(443)),
     heatTreatmentThread(HeatTreatmentThread::instance()),
     controller(new ThreadController)
{
   if (!MDNS.begin("cadovnik_esp8266")){
      Serial.println("failed to start MDNS");
   }
   MDNS.addService("https", "tcp", 443);
   controller->add(heatTreatmentThread);
   heatTreatmentThread->setInterval(100);
   server->setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
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

   server->on("/SetConvection", HTTP_POST, [this]()
   {
      int args = server->args();
      String val = server->arg(0);
      server->send(200, "application/json", heatTreatmentThread->setConvection(val == "true" ? true : false));
   });

   server->on("/SetAirPump", HTTP_POST, [this]()
   {
      int args = server->args();
      String val = server->arg(0);
      server->send(200, "application/json", heatTreatmentThread->setAirPump(val == "true" ? true : false));
   });

   server->on("/SetWaterPump", HTTP_POST, [this]()
   {
      int args = server->args();
      String val = server->arg(0);
      server->send(200, "application/json", heatTreatmentThread->setWaterPump(val == "true" ? true : false));
   });

   server->on("/StartHeatTreatment", HTTP_POST, [this]()
   {
      String respose = "{started : false}";
      if ( controller->shouldRun() )
      {
         heatTreatmentThread->setStart(true);
         controller->add(heatTreatmentThread);
         respose = "{started : true}";
      }
      server->send(200, "application/json", respose);
   });

   server->on("/SetMode", HTTP_POST, [this]()
   {
      int args = server->args();
      String val = server->arg(0);
      server->send(200, "application/json", heatTreatmentThread->setMode(val));
   });

   server->onNotFound([this]()
   {
      this->handleNotFound();
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