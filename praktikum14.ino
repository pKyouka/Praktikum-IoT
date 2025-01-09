#include  "WiFi.h" 

WiFiClient client;

#include "DHTesp.h"

const int DHT_PIN = 15;

DHTesp dhtSensor;;

String thingSpeakAddress = "api.thingspeak.com";
String writeAPIKey;
String tsfield1Name;
String request_string;


void setup()
{
  Serial.begin(9600);
  WiFi.disconnect();
  WiFi.begin("Wokwi-GUEST", "");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}


void loop()
{
  delay(2000);
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();

  float t = data.temperature;
  float h = data.humidity;

  kirim_thingspeak(t, h);

  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void kirim_thingspeak(float suhu, float hum) {
  if (client.connect("api.thingspeak.com", 80)) {
    request_string = "/update?";
    request_string += "key=";
    request_string += "ADQJU9DJ1DA296FR";
    request_string += "&";
    request_string += "field1";
    request_string += "=";
    request_string += suhu;
    request_string += "&";
    request_string += "field2";
    request_string += "=";
    request_string += hum;

    Serial.println(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
                 
    client.print(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

  }
}