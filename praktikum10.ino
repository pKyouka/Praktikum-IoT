#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h> // Untuk ESP8266
#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiClientSecure.h>        // Untuk ESP32
#endif

#include <HTTPClient.h>

#define WIFI_SSID           "Wokwi-GUEST"
#define WIFI_PASSWORD       ""

#define SERIAL_DEBUG_BAUD   115200

#define LED 15

#define ACCESSKEY "5599c02fd386b4f9:e68d97895d0962dd" // Ganti dengan access key akun Antares Anda
#define applicationName "PraktikumInternetofThings"   // Ganti dengan application name Antares yang telah dibuat
#define deviceName "praktikum10"                     // Ganti dengan device Antares yang telah dibuat

WiFiClientSecure client;

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to AP");
}

void reconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nReconnected to WiFi");
  }
}

void sendDataToAntares(int temp, int hum) {
  if (WiFi.status() == WL_CONNECTED) {
    client.setInsecure(); // Hanya untuk testing. Gunakan sertifikat jika memungkinkan.

    HTTPClient http;

    // URL ke Antares
    String serverPath = "https://platform.antares.id:8443/~/antares-cse/antares-id/" + String(applicationName) + "/" + String(deviceName);

    http.begin(client, serverPath);
    http.addHeader("X-M2M-Origin", ACCESSKEY);
    http.addHeader("Content-Type", "application/json;ty=4");

    // JSON payload (perbaikan di sini)
    String jsonPayload = "{";
    jsonPayload += "\"m2m:cin\":{";
    jsonPayload += "\"con\":\""; // `con` harus berupa string
    jsonPayload += "{";
    jsonPayload += "\\\"temperature\\\":" + String(temp) + ","; // Escape karakter untuk string
    jsonPayload += "\\\"humidity\\\":" + String(hum) + ",";
    jsonPayload += "}";
    jsonPayload += "\"}";
    jsonPayload += "}";

    Serial.println("Payload:");
    Serial.println(jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully!");
      Serial.println("Response: " + http.getString());
    } else {
      Serial.println("Error sending data. HTTP response code: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);
  Serial.println();
  InitWiFi();
  pinMode(LED, OUTPUT);
}

void loop() {
  delay(2000);

  int temp = random(25, 30);
  int hum = random(75, 90);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  } else {
    sendDataToAntares(temp, hum);
  }
}
