#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"

// ================= CONFIG =================
#define WIFI_SSID     "NAMA_WIFI"
#define WIFI_PASSWORD "PASSWORD_WIFI"

#define BOT_TOKEN "ISI_TOKEN_BOT"
#define CHAT_ID   "ISI_CHAT_ID"

// Sensor
#define DHTPIN 4
#define DHTTYPE DHT22     // pakai DHT22

#define FLAME_PIN 5       // sensor api / panas (digital)
// ==========================================

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastTimeBotRan;
const unsigned long botDelay = 1000; // cek pesan tiap 1 detik

// ================= HANDLE COMMAND =================
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text    = bot.messages[i].text;

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "❌ Akses ditolak", "");
      continue;
    }

    // ====== /start ======
    if (text == "/start") {
      String welcome = "🤖 *Monitoring ESP32*\n\n";
      welcome += "Perintah tersedia:\n";
      welcome += "/suhu - Cek suhu & kelembapan\n";
      welcome += "/kebakaran - Cek sensor panas/api\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    // ====== /suhu ======
    if (text == "/suhu") {
      float suhu = dht.readTemperature();
      float kelembapan = dht.readHumidity();

      if (isnan(suhu) || isnan(kelembapan)) {
        bot.sendMessage(chat_id, "❌ Gagal membaca sensor DHT22", "");
        return;
      }

      String pesan = "🌡 *Data DHT22*\n";
      pesan += "Suhu : " + String(suhu) + " °C\n";
      pesan += "Kelembapan : " + String(kelembapan) + " %";

      bot.sendMessage(chat_id, pesan, "Markdown");
    }

    // ====== /kebakaran ======
    if (text == "/kebakaran") {
      int flameState = digitalRead(FLAME_PIN);

      if (flameState == LOW) {
        bot.sendMessage(chat_id, "🔥 *PERINGATAN!*\nApi / panas terdeteksi!", "Markdown");
      } else {
        bot.sendMessage(chat_id, "✅ Aman\nTidak ada api atau panas berlebih.", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(FLAME_PIN, INPUT);
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
  if (millis() > lastTimeBotRan + botDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}
