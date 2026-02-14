#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "DHT.h"

// WiFi
#define WIFI_SSID "ir"
#define WIFI_PASSWORD "irsensor"

// Firebase
#define API_KEY "AIzaSyDnUeG2gHzirNQLEWmIVb3B0iSOKBndS88"
#define DATABASE_URL "https://irsensor-7092f-default-rtdb.firebaseio.com"

#define USER_EMAIL "pragatikore0118@gmail.com"
#define USER_PASSWORD "Pk@0106"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// IR Sensor
#define IR_PIN 4

// DHT Sensor
#define DHTPIN 15
#define DHTTYPE DHT11   // Change to DHT22 if needed
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic Sensor
#define TRIG_PIN 5
#define ECHO_PIN 18

long duration;
float distance;

void setup() {
  Serial.begin(115200);

  pinMode(IR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();

  // WiFi Connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {

  // 🔴 IR Sensor
  int irStatus = digitalRead(IR_PIN);

  // 🌡️ DHT Sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // 📏 Ultrasonic Sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Serial Monitor Output
  Serial.println("----- Sensor Data -----");
  Serial.print("IR Status: ");
  Serial.println(irStatus);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Firebase Upload
  if (Firebase.ready()) {
    Firebase.RTDB.setInt(&fbdo, "/Sensors/IR", irStatus);
    Firebase.RTDB.setFloat(&fbdo, "/Sensors/Temperature", temperature);
    Firebase.RTDB.setFloat(&fbdo, "/Sensors/Humidity", humidity);
    Firebase.RTDB.setFloat(&fbdo, "/Sensors/Distance", distance);
  }

  delay(2000);
}
