#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"

// ================= CONFIG =================
#define WIFI_SSID "NAMA_WIFI"
#define WIFI_PASSWORD "PASSWORD_WIFI"

#define BOT_TOKEN "ISI_TOKEN_BOT"
#define CHAT_ID "ISI_CHAT_ID"

#define DHTPIN 4
#define DHTTYPE DHT11   // ganti DHT22 jika pakai DHT22
// ==========================================

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi terhubung!");
  secured_client.setInsecure();
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca sensor DHT!");
    return;
  }

  String pesan = "📡 *Monitoring DHT*\n";
  pesan += "🌡 Suhu : " + String(suhu) + " °C\n";
  pesan += "💧 Kelembapan : " + String(kelembapan) + " %";

  bot.sendMessage(CHAT_ID, pesan, "Markdown");

  Serial.println("Data terkirim ke Telegram");
  delay(60000); // kirim tiap 1 menit
}
