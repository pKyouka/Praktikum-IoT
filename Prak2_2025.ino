#include "DHT.h"

#define DHTPIN 12
#define DHTTYPE DHT22
#define LEDPIN 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" *C  ");
  Serial.print("Kelembapan: ");
  Serial.print(humidity);
  Serial.println(" %");

  float batasSuhu = 30.0;
  float batasLembab = 80.0;

  if (temperature > batasSuhu || humidity > batasLembab) {
    digitalWrite(LEDPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
  }
}
