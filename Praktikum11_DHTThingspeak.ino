#include <WiFi.h>
#include <DHT.h>
#include "ThingSpeak.h"

// ====== KONFIGURASI WIFI ======
const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

// ====== KONFIGURASI THINGSPEAK ======
unsigned long channelID = 1234567;       // GANTI
const char* writeAPIKey = "WRITE_API_KEY";

// ====== KONFIGURASI DHT ======
#define DHTPIN 4          // GPIO tempat DATA DHT
#define DHTTYPE DHT22     // GANTI ke DHT11 jika pakai DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Inisialisasi DHT
  dht.begin();

  // Koneksi WiFi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Terhubung!");

  // Inisialisasi ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Baca sensor
  float kelembapan = dht.readHumidity();
  float suhu = dht.readTemperature(); // Celsius

  // Cek apakah pembacaan gagal
  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca DHT!");
    delay(2000);
    return;
  }

  Serial.println("Data Sensor:");
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" °C");

  Serial.print("Kelembapan: ");
  Serial.print(kelembapan);
  Serial.println(" %");

  // Kirim ke ThingSpeak
  ThingSpeak.setField(1, suhu);
  ThingSpeak.setField(2, kelembapan);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (status == 200) {
    Serial.println("Data berhasil dikirim ke ThingSpeak");
  } else {
    Serial.print("Gagal kirim data. Error code: ");
    Serial.println(status);
  }

  // Delay minimal ThingSpeak
  delay(15000);
}
