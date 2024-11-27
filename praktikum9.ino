#include <WiFi.h>

const int ledHijau = 26;
const int ledBiru = 27;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const int wifiChannel = 6;

void setup() {
    Serial.begin(9600);
    pinMode(ledHijau, OUTPUT);
    pinMode(ledBiru, OUTPUT);
    digitalWrite(ledHijau, LOW);
    digitalWrite(ledBiru, LOW);

    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password, wifiChannel);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println(" Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    digitalWrite(ledHijau, HIGH);
    digitalWrite(ledBiru, LOW);
    delay(1000);
    digitalWrite(ledHijau, LOW);
    digitalWrite(ledBiru, HIGH);
    delay(1000);
}
