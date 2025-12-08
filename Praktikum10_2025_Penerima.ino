#include <AntaresESP32MQTT.h>

#define ACCESSKEY       ""
#define WIFISSID        ""
#define PASSWORD        ""
#define applicationName ""
#define deviceName      ""

AntaresESP32MQTT antares(ACCESSKEY);

void callback(char topic[], byte payload[], unsigned int length) {
  antares.get(topic, payload, length);
  Serial.println(antares.getTopic());
  Serial.println(antares.getPayload());
  Serial.println(antares.getInt("temperature"));
  Serial.println(antares.getInt("humidity"));
}

void setup() {
  Serial.begin(115200);
  antares.setDebug(false);
  antares.wifiConnection(WIFISSID, PASSWORD);
  antares.setMqttServer();
  antares.setCallback(callback);
}

void loop() {
  antares.checkMqttConnection();
}
