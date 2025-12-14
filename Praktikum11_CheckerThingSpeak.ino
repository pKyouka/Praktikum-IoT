#include <WiFi.h>
#include <ThingSpeak.h>

// ====== GANTI SESUAI PUNYAMU ======
const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

unsigned long channelID = 1234567;      // Channel ID ThingSpeak
const char* writeAPIKey = "API_KEY_KAMU";
// =================================

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  ThingSpeak.begin(client);
}

void loop() {
  // ===== DATA DUMMY =====
  float temperature = random(250, 350) / 10.0; // 25.0 - 35.0 °C
  float humidity    = random(500, 800) / 10.0; // 50.0 - 80.0 %
  // ======================

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (status == 200) {
    Serial.println("Data berhasil dikirim!");
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
  } else {
    Serial.print("Gagal kirim data. HTTP error ");
    Serial.println(status);
  }

  delay(15000); // ThingSpeak minimal 15 detik
}
