#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

#include "creds.h"

// #define DONT_HAVE_SENSORS

#define MQ5PIN 		D6
#define DHTPIN 		D2
#define DHTTYPE		DHT11

#define BAUDRATE	115200

struct readings {
  bool gas;						// Gas status: true if gas detected and false if no gas detected
  float hum;  				// Humidity in Percent	  ( %)
  float temp;  				// Temperature in Celsius (°C)
} readings;

DHT dht(DHTPIN, DHTTYPE);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store

// will store last time data was sent to Firebase
unsigned long previousMillis = 0;
const long interval = 2000;

void connectToWiFi(char const *ssid, char const *password);
void readSensors(struct readings *r);
void displaySensors(struct readings r);
void sendDataToFirebase(struct readings r);

void setup() {
  pinMode(MQ5PIN, INPUT);
  Serial.begin(BAUDRATE);
  connectToWiFi(SSID, PASSWORD);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  dht.begin();
}

void loop() {
  // check to see if it's time to send data to Firebase; that is, if the difference
  // between the current time and last time we sent data is bigger than
  // the interval at which we want to send data.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time we sent data to Fireabase
    previousMillis = currentMillis;
    readSensors(&readings);
    displaySensors(readings);
    sendDataToFirebase(readings);
  }
}

void connectToWiFi(char const *ssid, char const *password) {
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
  would try to act as both a client and an access-point and could cause
  network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  //start connecting to WiFi
  WiFi.begin(ssid, password);
  //while client is not connected to WiFi keep loading
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

void readSensors(struct readings *r) {
  #ifdef DONT_HAVE_SENSORS
    readings.gas = !readings.gas;
    readings.temp = random(0, 80);
    readings.hum = random(0, 80);
  #else
    // Read Gas status
    r->gas = digitalRead(MQ5PIN);
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    r->hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    r->temp = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(r->hum) || isnan(r->temp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
  #endif
}

void displaySensors(struct readings r) {
  if (r.gas == true) {
    Serial.println("[INFO] Gas DETECTED!!!");
  }
  Serial.print("[INFO] Gas status: ");
  Serial.println(r.gas);
  Serial.print("[INFO] Humidity: ");
  Serial.print(r.hum);
  Serial.println("%");
  Serial.print("[INFO] Temperature: ");
  Serial.print(r.temp);
  Serial.print("°C ");
}

void sendDataToFirebase(struct readings r) {
  String gasStatusID = Firebase.pushInt("mq5/gas", r.gas);
  if (Firebase.failed()) {
    Serial.print("[ERROR] pushing mq5/gasStatus failed:");
    Serial.println(Firebase.error());
    return;
  }

  String humValueID = Firebase.pushFloat("dht11/humidity", r.hum);
  if (Firebase.failed()) {
    Serial.print("[ERROR] pushing /dht11/humidity failed:");
    Serial.println(Firebase.error());
    return;
  }

  String tempValueID = Firebase.pushFloat("dht11/temperature", r.temp);
  if (Firebase.failed()) {
    Serial.print("[ERROR] pushing /dht11/temperature failed:");
    Serial.println(Firebase.error());
    return;
  }
}