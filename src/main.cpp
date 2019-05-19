#include "Thread.h"
#include "ThreadController.h"
#include "webServer.h"

#define STASSID "ASUS"
#define STAPSK  "9163404043"

const char* ssid = STASSID;
const char* password = STAPSK;

ThreadController* controller;

void timerCallback()
{
	controller->run();
  delay(500); 
}
const static int relays[] = {RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4, RELAY_PIN_5, RELAY_PIN_6, RELAY_PIN_7};
void setup()
{
  controller = new ThreadController();
  WebServerThread* webServerThread = WebServerThread::instance();
  webServerThread->setInterval(10);
  controller->add(webServerThread);
  Serial.begin(9600);
   for ( auto i : relays )
      pinMode(i, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  timerCallback();
}
