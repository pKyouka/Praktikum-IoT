#define BLYNK_PRINT Serial

// ===== BLYNK CONFIG =====
#define BLYNK_TEMPLATE_ID   "TMPL6iNNUZW-c"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN    "KPm_QT_IRg3Mu3qD-uD88wYOQ1jmchJh"

// ===== LIBRARY =====
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// ===== WIFI CONFIG =====
char ssid[] = "NAMA_WIFI_KAMU";
char pass[] = "PASSWORD_WIFI_KAMU";

// ===== DHT CONFIG =====
#define DHTPIN D4        // Pin data DHT (GPIO2)
#define DHTTYPE DHT11    // Ganti ke DHT22 jika pakai DHT22

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// ===== FUNCTION BACA SENSOR =====
void sendDHTData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca sensor DHT!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C | Kelembapan: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Kirim ke Blynk
  Blynk.virtualWrite(V4, temperature); // SUHU
  Blynk.virtualWrite(V5, humidity);    // KELEMBAPAN
}

void setup() {
  Serial.begin(9600);

  dht.begin();

  // Koneksi ke Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Baca sensor setiap 2 detik
  timer.setInterval(2000L, sendDHTData);
}

void loop() {
  Blynk.run();
  timer.run();
}
