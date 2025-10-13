const int mq2AnalogPin = A0;
const int mq2DigitalPin = 8;
const int redLedPin = 13;

int gasThreshold = 400;

void setup() {
  Serial.begin(9600);
  pinMode(mq2DigitalPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  digitalWrite(redLedPin, LOW);
  delay(20000);
}

void loop() {
  int gasLevel = analogRead(mq2AnalogPin);
  int gasDigital = digitalRead(mq2DigitalPin);
  float voltage = gasLevel * (5.0 / 1023.0);

  Serial.print("Gas Level (Analog): ");
  Serial.print(gasLevel);
  Serial.print(" | Voltage: ");
  Serial.print(voltage);
  Serial.print("V | Digital Output: ");
  Serial.println(gasDigital == LOW ? "GAS DETECTED!" : "Normal");

  if (gasLevel > gasThreshold) {
    digitalWrite(redLedPin, HIGH);
  } else {
    digitalWrite(redLedPin, LOW);
  }

  delay(500)
