#define SENSOR_PIN A0

float lightIntensity;

void setup() {
    Serial.begin(115200);
    Serial.println("Inisialisasi sensor photodioda...");
}

void loop() {
    int rawValue = analogRead(SENSOR_PIN);
    lightIntensity = (rawValue / 1023.0) * 1000.0;

    Serial.print("Intensitas Cahaya: ");
    Serial.print(lightIntensity);
    Serial.println(" lux");

    delay(1000);
}
