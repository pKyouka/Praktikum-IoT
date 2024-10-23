#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int DHT_PIN = 4;
const int pinled = 5;
DHTesp dhtSensor;
float suhu = 0;
float kelembaban = 0;

void setup() {
    Serial.begin(115200);
    dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
    pinMode(pinled, OUTPUT);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(3, 0);
    lcd.print("Monitoring");
    lcd.setCursor(2, 1);
    lcd.print("Suhu & Kelembaban");
    delay(3000);
    lcd.clear();
}

void loop() {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    suhu = data.temperature;
    kelembaban = data.humidity;

    Serial.print("Temp: ");
    Serial.print(suhu);
    Serial.println("C");
    Serial.print("Hum: ");
    Serial.print(kelembaban);
    Serial.println("%");

    delay(2000);

    if (suhu < 25) {
        digitalWrite(pinled, HIGH);
    } else {
        digitalWrite(pinled, LOW);
    }

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 2);
    lcd.print("Suhu       :");
    lcd.setCursor(13, 2);
    lcd.print(suhu);
    lcd.setCursor(19, 2);
    lcd.print("C");
    lcd.setCursor(0, 3);
    lcd.print("Kelembaban :");
    lcd.setCursor(13, 3);
    lcd.print(kelembaban);
    lcd.setCursor(19, 3);
    lcd.print("%");
    delay(8000);
}
