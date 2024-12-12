#include <Wire.h>

// Alamat I2C sensor photodioda (sesuaikan jika berbeda)
#define SENSOR_ADDRESS 0x39

// Variabel untuk menyimpan data dari sensor
float lightIntensity; // Intensitas cahaya yang diterima
float algaeConcentration; // Konsentrasi alga yang dihitung

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
    // Analisis konsentrasi alga menggunakan rumus
    // Misalnya: algaeConcentration = (lightIntensity / nilaiReferensi) * faktorKoreksi;
    algaeConcentration = lightIntensity * 0.05; // Sesuaikan formula dengan kebutuhan Anda

    // Menampilkan data ke serial monitor
    Serial.print("Intensitas Cahaya: ");
    Serial.print(lightIntensity);
    Serial.print(" lux, Konsentrasi Alga: ");
    Serial.print(algaeConcentration);
    Serial.println(" mg/L");

    // Tambahkan logika tambahan jika diperlukan (misal, kirim data ke server IoT)
  } else {
    Serial.println("Gagal membaca data dari sensor.");
  }

  delay(1000); // Delay 1 detik
}
