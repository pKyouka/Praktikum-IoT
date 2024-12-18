
#include <WiFi.h>
#include "PubSubClient.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "mqtt.antares.id";

// Define the access key, application, and device names
const char* access_key = "5599c02fd386b4f9:e68d97895d0962dd";
const char* Application = "PraktikumInternetofThings";
const char* Device = "praktikum11";

// MQTT topics
char publishTopic[200]; 
char subscribeTopic[200]; 


char msg[300];
int port = 1883;
String stMac;
char mac[50];
char clientId[50];

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
int value = 0;

const int ledPin = 5;

#include "DHTesp.h"
const int DHT_PIN = 15;
DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  sprintf(publishTopic, "/oneM2M/req/%s/antares-cse/json", access_key);
  sprintf(subscribeTopic, "/oneM2M/resp/antares-cse/%s/json", access_key);
  
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  wifiConnect();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  stMac = WiFi.macAddress();
  stMac.replace(":", "_");
  Serial.println(stMac);
  
  client.setServer(mqttServer, port);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    long r = random(1000);
    sprintf(clientId, "clientId-%ld", r);
    if (client.connect(access_key)) {
      Serial.print(clientId);
      Serial.println(" connected");
      client.subscribe(publishTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String stMessage;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    stMessage += (char)message[i];
  }
  Serial.println();

  if (String(topic) == subscribeTopic) {
    Serial.print("Changing output to ");
    if(stMessage == "LED ON"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(stMessage == "LED OFF"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void loop() {
  delay(10);
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();
    int suhu = data.temperature;
    int kelembaban = data.humidity;
    Serial.print("publish topic : ");
    Serial.println(publishTopic);
    Serial.println("Temp: " + String(suhu) + "°C");
    Serial.println("Humidity: " + String(kelembaban, 1) + "%");
    Serial.println("---");
    lastMsg = now;
    client.subscribe(publishTopic);
    Serial.print("Publish Topic: ");
    Serial.println(publishTopic);
    sprintf(msg,
      "{\"m2m:rqp\": {\"fr\": \"%s\", \"to\": \"/antares-cse/antares-id/%s/%s\", \"op\": 1, \"rqi\": 123456, \"pc\": {\"m2m:cin\": {\"cnf\": \"message\", \"con\": \"{\\\"temp\\\":%d, \\\"status\\\": 1}\"}}, \"ty\": 4}}",
      access_key, Application, Device, suhu);
    
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(publishTopic, msg);

  }
}
