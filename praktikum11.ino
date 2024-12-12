#if defined(ESP32)
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#define WIFI_SSID           "Wokwi-GUEST"
#define WIFI_PASSWORD       ""
#define SERIAL_DEBUG_BAUD   115200
#define PIR_PIN             14
#define ACCESSKEY           "5599c02fd386b4f9:e68d97895d0962dd" // Ganti dengan access key akun Antares Anda
#define applicationName     "PraktikumInternetofThings"         // Ganti dengan application name Antares yang telah dibuat
#define deviceName          "praktikum11"                       // Ganti dengan device Antares yang telah dibuat

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

void sendDataToAntares(const char* motionStatus) {
  if (WiFi.status() == WL_CONNECTED) {
    client.setInsecure(); // Hanya untuk testing. Gunakan sertifikat jika memungkinkan.

    HTTPClient http;

    // URL ke Antares
    String serverPath = "https://platform.antares.id:8443/~/antares-cse/antares-id/" + String(applicationName) + "/" + String(deviceName);

    http.begin(client, serverPath);
    http.addHeader("X-M2M-Origin", ACCESSKEY);
    http.addHeader("Content-Type", "application/json;ty=4");

    // JSON payload
    String jsonPayload = "{";
    jsonPayload += "\"m2m:cin\":{";
    jsonPayload += "\"con\":\""; // `con` harus berupa string
    jsonPayload += motionStatus;
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
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  delay(1000); // Cek setiap 1 detik

  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    Serial.println("Motion detected!");
    sendDataToAntares("Motion detected");
  } else {
    Serial.println("No motion detected.");
    sendDataToAntares("No motion detected");
  }

  delay(5000); // Kirim data setiap 5 detik
}
#endif