#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

DHT dht1(8, DHT22);
DHT dht2(9, DHT22);
DHT dht3(10, DHT22);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int button1Pin = 3;
const int button2Pin = 2;
const int led1Pin = 13;
const int led2Pin = 12;
const int led3Pin = 11;
int currentSensor = 1;
int displayAverage = 0;

void setup() {
    dht1.begin();
    dht2.begin();
    dht3.begin();
    lcd.init();
    lcd.backlight();
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);
    pinMode(led3Pin, OUTPUT);
}

void loop() {
    float c1 = dht1.readTemperature();
    float c2 = dht2.readTemperature();
    float c3 = dht3.readTemperature();
    float h1 = dht1.readHumidity();
    float h2 = dht2.readHumidity();
    float h3 = dht3.readHumidity();

    if (digitalRead(button2Pin) == LOW) {
        displayAverage++;
        if (displayAverage > 1) {
            displayAverage = 0;
        }
    }

    if (displayAverage == 0) {
        if (digitalRead(button1Pin) == LOW) {
            delay(100);
            currentSensor += 1;
            if (currentSensor > 3) {
                currentSensor = 1;
            }
        }

        switch (currentSensor) {
            case 1:
                lcd.setCursor(0, 0);
                lcd.print("DHT1 Temp: " + String(c1) + " C   ");
                lcd.setCursor(0, 1);
                lcd.print("Humidity: " + String(h1) + " %   ");
                break;
            case 2:
                lcd.setCursor(0, 0);
                lcd.print("DHT2 Temp: " + String(c2) + " C   ");
                lcd.setCursor(0, 1);
                lcd.print("Humidity: " + String(h2) + " %   ");
                break;
            case 3:
                lcd.setCursor(0, 0);
                lcd.print("DHT3 Temp: " + String(c3) + " C   ");
                lcd.setCursor(0, 1);
                lcd.print("Humidity: " + String(h3) + " %   ");
                break;
        }
    }

    if (displayAverage == 1) {
        float averageTemp = (c1 + c2 + c3) / 3;
        float averageHumidity = (h1 + h2 + h3) / 3;
        lcd.setCursor(0, 0);
        lcd.print("Avg Temp: " + String(averageTemp) + " C   ");
        lcd.setCursor(0, 1);
        lcd.print("Avg Humi: " + String(averageHumidity) + " %   ");
    }

    if (c1 > 35) {
        digitalWrite(led1Pin, HIGH);
    } else {
        digitalWrite(led1Pin, LOW);
    }

    if (c2 > 35) {
        digitalWrite(led2Pin, HIGH);
    } else {
        digitalWrite(led2Pin, LOW);
    }

    if (c3 > 35) {
        digitalWrite(led3Pin, HIGH);
    } else {
        digitalWrite(led3Pin, LOW);
    }
}