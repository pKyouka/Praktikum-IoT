#include <Wire.h>

// Alamat I2C sensor photodioda (sesuaikan dengan modul Anda)
#define SENSOR_ADDRESS 0x39

// Variabel untuk menyimpan data dari sensor
float lightIntensity; // Intensitas cahaya yang diterima
float nitrateConcentration; // Konsentrasi Nitrat (mg/L)
float phosphateConcentration; // Konsentrasi Fosfat (mg/L)

// Variabel tambahan untuk perhitungan
float volumeMedia = 10.0; // Volume media dalam liter
float purityNitrate = 100.0; // Kemurnian KNO3 (%)
float purityPhosphate = 100.0; // Kemurnian KH2PO4 (%)

// Fungsi untuk membaca data dari sensor photodioda
float readLightIntensity() {
  Wire.beginTransmission(SENSOR_ADDRESS);
  Wire.write(0x00); // Perintah membaca data (sesuaikan dengan datasheet sensor Anda)
  Wire.endTransmission();
  Wire.requestFrom(SENSOR_ADDRESS, 2); // Meminta 2 byte data

  if (Wire.available() == 2) {
    int highByte = Wire.read();
    int lowByte = Wire.read();
    return (highByte << 8) | lowByte; // Menggabungkan dua byte data
  }
  return -1; // Gagal membaca
}

void setup() {
  // Inisialisasi komunikasi serial dan I2C
  Serial.begin(115200);
  Wire.begin(D2, D1); // SDA ke D2, SCL ke D1 (sesuaikan dengan NodeMCU)

  // Inisialisasi sensor
  Serial.println("Inisialisasi sensor photodioda...");
}

void loop() {
  // Membaca intensitas cahaya dari sensor
  lightIntensity = readLightIntensity();

  if (lightIntensity >= 0) {
    // Analisis konsentrasi nutrisi (Nitrat dan Fosfat)
    // Misalnya: mapping intensitas cahaya ke konsentrasi
    nitrateConcentration = lightIntensity * 0.02; // Nitrat (mg/L) - Sesuaikan formula
    phosphateConcentration = lightIntensity * 0.01; // Fosfat (mg/L) - Sesuaikan formula

    // Hitung massa zat berdasarkan rumus
    float nitrateMass = (nitrateConcentration * volumeMedia) / purityNitrate;
    float phosphateMass = (phosphateConcentration * volumeMedia) / purityPhosphate;

    // Menampilkan hasil ke serial monitor
    Serial.println("===================================================");
    Serial.print("Intensitas Cahaya: ");
    Serial.print(lightIntensity);
    Serial.println(" lux");
    
    Serial.print("Konsentrasi Nitrat (mg/L): ");
    Serial.println(nitrateConcentration);
    Serial.print("Massa KNO3 yang dibutuhkan (g): ");
    Serial.println(nitrateMass);

    Serial.print("Konsentrasi Fosfat (mg/L): ");
    Serial.println(phosphateConcentration);
    Serial.print("Massa KH2PO4 yang dibutuhkan (g): ");
    Serial.println(phosphateMass);

    Serial.println("===================================================");
  } else {
    Serial.println("Gagal membaca data dari sensor.");
  }

  delay(1000); // Delay 1 detik
}
