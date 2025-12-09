#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ---------- GANTI DENGAN PUNYAMU SENDIRI ----------
#define WIFI_SSID   "" //GANTI PAKE YANG BENERAN 
#define WIFI_PASS   "" //GANTI PAKE YANG BENERAN

#define MQTT_SERVER "mqtt.antares.id"   // broker Antares
#define MQTT_PORT   1883

// Contoh: topic untuk subscribe data dari suatu device.
// Sementara kita pakai contoh custom topic saja, sesuaikan dengan punyamu.

//GANTI YANG TENGAH JADI ACCESS KEY KALIAN 
// 88eee28c6fd062bc:d2f52ab1c5461a58 yang ini jadi ACCESS KEY KALIAN SEBELUMNYA
//#define MQTT_TOPIC_SUB "/oneM2M/resp/antares-cse/88eee28c6fd062bc:d2f52ab1c5461a58/json"

// ---------------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

// Fungsi kalau ada pesan MQTT masuk
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("=== Pesan MQTT diterima ===");
  Serial.print("Topic: ");
  Serial.println(topic);

  // Ubah payload jadi string dulu
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Payload String: ");
  Serial.println(msg);

  // Parsing JSON
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, msg);

  if (error) {
    Serial.print("Gagal parse JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Contoh ambil value dari JSON
  // Sesuaikan dengan struktur JSON yang kamu kirim dari Antares
  if (doc.containsKey("temperature")) {
    float temperature = doc["temperature"];
    Serial.print("Temperature: ");
    Serial.println(temperature);
  }

  if (doc.containsKey("humidity")) {
    float humidity = doc["humidity"];
    Serial.print("Humidity: ");
    Serial.println(humidity);
  }

  Serial.println("===========================");
}

void reconnect() {
  // Loop sampai terkoneksi ke MQTT
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    // Client ID bisa bebas, yang penting unik
    String clientId = "ESP32-Sub-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Terhubung!");

      // Subscribe topic
      client.subscribe(MQTT_TOPIC_SUB);
      Serial.print("Subscribe ke topic: ");
      Serial.println(MQTT_TOPIC_SUB);

    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi 5 detik...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Koneksi WiFi
  Serial.println();
  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi terhubung.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup MQTT
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
