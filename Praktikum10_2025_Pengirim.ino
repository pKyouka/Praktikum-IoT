#include <AntaresESP32MQTT.h>

#define ACCESSKEY       ""        
#define WIFISSID        ""         
#define PASSWORD        ""     
#define applicationName ""
#define deviceName      ""

AntaresESP32MQTT antares(ACCESSKEY);

void setup() {
  Serial.begin(115200);
  antares.setDebug(false);
  antares.wifiConnection(WIFISSID, PASSWORD);
  antares.setMqttServer();
}

void loop() {
  antares.checkMqttConnection();

  int temperature = random(25, 31);
  int humidity    = random(60, 91);

  antares.add("temperature", temperature);
  antares.add("humidity", humidity);

  antares.publish(applicationName, deviceName);

  delay(5000);
}
