#include <Servo.h>

Servo myServo;

const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 12;

void setup() {
    Serial.begin(9600);
    myServo.attach(11);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
}

void loop() {
    long duration, distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.034) / 2;

    Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 20) {
        myServo.write(180);
        tone(buzzerPin, 1000);
    } else {
        myServo.write(0);
        noTone(buzzerPin);
    }

    delay(500);
}
