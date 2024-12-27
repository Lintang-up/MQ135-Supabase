#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// Konfigurasi WiFi
const char* ssid = "P1";
const char* password = "69696969";

// URL dan API Key Supabase
const String supabaseURL = "https://gzoknwjgzhjowuzzcixr.supabase.co";
const String apiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Imd6b2tud2pnemhqb3d1enpjaXhyIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzUyMzI1NDksImV4cCI6MjA1MDgwODU0OX0.NqpV90BIsMw00S4EfGDcRvQzconuaYo3wIqxmLjUFJU";

// Pin sensor MQ135
const int mq135Pin = A0;

HTTPClient https;
WiFiClientSecure client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nTerhubung ke WiFi");

   client.setInsecure();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Membaca nilai sensor
    int sensorValue = analogRead(mq135Pin);
    Serial.print("Nilai MQ135    : ");
    Serial.println(sensorValue);

    // Menyusun data JSON untuk dikirim
    String postData = "{\"nilai\":" + String(sensorValue) + "}";

    // Mengirim permintaan POST ke Supabase
    https.begin(client, supabaseURL + "/rest/v1/sensor");
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Prefer", "return=representation");
    https.addHeader("apikey", apiKey);
    https.addHeader("Authorization", "Bearer " + apiKey);

    // Mengirim data JSON
    int httpCode = https.POST(postData);
    String payload = https.getString();

    // Menampilkan hasil HTTP
    Serial.print("Kode HTTPS     : ");
    Serial.println(httpCode);
    Serial.print("Hasil Databse  : ");
    Serial.println(payload);
    Serial.print("----------------------------------------------------------------------------\n");

    https.end(); // Menutup koneksi
  } else {
    Serial.println("WiFi tidak terhubung");
  }

  delay(5000); // Kirim data setiap 10 detik
}
