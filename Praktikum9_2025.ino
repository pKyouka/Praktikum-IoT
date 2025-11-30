#include <AntaresESP32HTTP.h> // Memanggil library Antares HTTP

// --- KONFIGURASI JARINGAN & ANTARES ---
#define ACCESSKEY "ISI_DENGAN_ACCESS_KEY_ANDA" // AKSES ID NYA JANGAN LUPA DIGANTI
#define WIFISSID  "NAMA_WIFI_ANDA" // JANGAN LUPA DIGANTI
#define PASSWORD  "PASSWORD_WIFI_ANDA" // JANGAN LUPA DIGANTI 
#define projectName "ISI_NAMA_APLIKASI_ANTARES" // Contoh: PraktikumIOT_UNISA
#define deviceName  "ISI_NAMA_DEVICE_ANTARES"   // Contoh: ESP32_Saya

AntaresESP32HTTP antares(ACCESSKEY); // Inisialisasi objek Antares

void setup() {
  Serial.begin(115200); // Memulai komunikasi serial
  antares.setDebug(true); // Menampilkan log debug Antares di Serial Monitor
  antares.wifiConnection(WIFISSID, PASSWORD); // Menghubungkan ke Wi-Fi
}

void loop() {
  // --- BAGIAN SIMULASI SENSOR (TANPA ALAT FISIK) ---
  // Kita membuat angka acak untuk seolah-olah menjadi data sensor
  int suhuDummy = random(25, 35);      // Angka acak antara 25 s.d 35
  int kelembapanDummy = random(60, 90); // Angka acak antara 60 s.d 90

  // Menambahkan variabel data ke paket pengiriman
  antares.add("suhu", suhuDummy);
  antares.add("kelembapan", kelembapanDummy);
  
  // Mengirim paket data ke cloud Antares
  // Format: antares.send(NamaAplikasi, NamaDevice)
  antares.send(projectName, deviceName);

  // Delay 10 detik sebelum mengirim data lagi
  // (Jangan terlalu cepat agar tidak terkena limit server)
  delay(10000); 
}
