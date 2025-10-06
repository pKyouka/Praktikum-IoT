#define TRIG_PIN 12
#define ECHO_PIN 11

long duration;
float distance_cm;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance_cm = (duration * 0.0343) / 2;

  Serial.print("Jarak: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
}
