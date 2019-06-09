#include "Thread.h"
#include "ThreadController.h"
#include "webServer.h"

#define STASSID "AndroidAP"
#define STAPSK  "nowi7765"

const char* ssid = STASSID;
const char* password = STAPSK;

ThreadController* controller;

void timerCallback()
{
	controller->run();
  delay(500); 
}

const static int relays[] = {RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4, RELAY_PIN_5, RELAY_PIN_6};

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup()
{
  controller = new ThreadController();
  WebServerThread* webServerThread = WebServerThread::instance();
  webServerThread->setInterval(10);
  controller->add(webServerThread);
  Serial.begin(9600);
   for ( auto i : relays )
      pinMode(i, OUTPUT);
  

  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  WiFi.softAP("Smokehouse", "9163404043");
  // WiFi.begin(ssid, password);

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }

  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  timerCallback();
}
