#define SENSOR_ANALOG_PIN A0
#define SENSOR_DIGITAL_PIN D0

float lightIntensity;
float algaeConcentration;

float readLightIntensity() {
    int rawValue = analogRead(SENSOR_ANALOG_PIN);
    return (rawValue / 1023.0) * 1000.0;
}

void setup() {
    Serial.begin(115200);
    pinMode(SENSOR_DIGITAL_PIN, INPUT);
    Serial.println("Inisialisasi sensor photodioda...");
}

void loop() {
    lightIntensity = readLightIntensity();
    int digitalStatus = digitalRead(SENSOR_DIGITAL_PIN);
    algaeConcentration = lightIntensity * 0.05;

    Serial.print("Intensitas Cahaya: ");
    Serial.print(lightIntensity);
    Serial.print(" lux");

    if (digitalStatus == HIGH) {
        Serial.print(", Status: Batas Terpenuhi");
    } else {
        Serial.print(", Status: Normal");
    }

    Serial.print(", Konsentrasi Alga: ");
    Serial.print(algaeConcentration);
    Serial.println(" mg/L");

    delay(1000);
}
