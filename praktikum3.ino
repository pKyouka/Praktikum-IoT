#define ledPinIndoor 13
#define pirPin A0
#define ledPinOutdoor 10
#define ldrPin A1
#define trigPin 9
#define echoPin 8

int pirState = 0;
int valPir = 0;
long duration;
int distance;

void setup() {
    pinMode(ledPinIndoor, OUTPUT);
    pinMode(pirPin, INPUT);
    pinMode(ledPinOutdoor, OUTPUT);
    pinMode(ldrPin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    int valPir = digitalRead(pirPin);
    int statusLdr = analogRead(ldrPin);

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    if (valPir == 1) {
        digitalWrite(ledPinIndoor, HIGH);
        if (pirState == 1) {
            Serial.println("ADA ORANG, Dalam Rumah!");
            pirState = 0;
        }
    } else {
        digitalWrite(ledPinIndoor, LOW);
        if (pirState == 0) {
            Serial.println("TIDAK ADA ORANG, Di Dalam Rumah!");
            pirState = 1;
        }
    }

    if (statusLdr > 300) {
        digitalWrite(ledPinOutdoor, HIGH);
        Serial.print("MALAM, Lampu di Luar Rumah Hidup :");
        Serial.println(statusLdr);
    } else {
        digitalWrite(ledPinOutdoor, LOW);
        Serial.print("SIANG, Lampu di Luar Rumah Mati :");
        Serial.println(statusLdr);
    }

    Serial.print("Jarak: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.println(" ");

    delay(2000);
}
