#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>
#include "Creds.h"

// comment this line if you want to use DHT11
#define TEST

#define DHTTYPE DHT11
#define DHTPIN D2

DHT dht(DHTPIN, DHTTYPE);

void connectToWiFi() {
    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(SSID);
    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
    would try to act as both a client and an access-point and could cause
    network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    //start connecting to WiFi
    WiFi.begin(SSID, PASSWORD);
    //while client is not connected to WiFi keep loading
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected to ");
    Serial.println(SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
}

void setup() {
    Serial.begin(115200);
    connectToWiFi();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    dht.begin();
}

void loop() {
    #ifdef TEST
        // === Push temperature value to Firebase ===
        String tempValueID = Firebase.pushInt("dht11/temperature", random(0, 80));
        Serial.print("[INFO] temperature: ");
        Serial.println(random(0, 80));
        if (Firebase.failed()) {
            Serial.print("[ERROR] pushing /dht11/temperature failed:");
            Serial.println(Firebase.error());
            return;
        }
        Serial.print("[INFO] pushed: /dht11/temperature \tkey: ");
        Serial.println(tempValueID);
        
        // === Push humidity value to Firebase ===
        String humValueID = Firebase.pushInt("dht11/humidity", random(0, 80));
        Serial.print("[INFO] humidity: ");
        Serial.println(random(0, 80));
        if (Firebase.failed()) {
            Serial.print("[ERROR] pushing /dht11/humidity failed:");
            Serial.println(Firebase.error());
            return;
        }
        Serial.print("[INFO] pushed: /dht11/humidity    \tkey: ");
        Serial.println(humValueID);
        Serial.println();
    #else
        // === Read and Log temperature and humidity to Serial Monitor ===
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        Serial.print("[INFO] Current Humidity = ");
        Serial.print(h);
        Serial.println(" %");
        Serial.print("[INFO] Current Temperature = ");
        Serial.print(t);
        Serial.println(" °C");
        
        // Eliminate abnormal values
        if ((t >= -15 && t <= 80) && (h >= 0 && h <= 100)) {
            // === Push temperature value to Firebase ===
            String tempValueID = Firebase.pushFloat("dht11/temperature", t);
            if (Firebase.failed()) {
                Serial.print("[ERROR] pushing /dht11/temperature failed:");
                Serial.println(Firebase.error());
                return;
            }
            Serial.print("[INFO] pushed: /dht11/temperature \tkey: ");
            Serial.println(tempValueID);

            // === Push humidity value to Firebase ===
            String humValueID = Firebase.pushFloat("dht11/humidity", h);
            if (Firebase.failed()) {
                Serial.print("[ERROR] pushing /dht11/humidity failed:");
                Serial.println(Firebase.error());
                return;
            }
            Serial.print("[INFO] pushed: /dht11/humidity    \tkey: ");
            Serial.println(humValueID);
            Serial.println();
        } else {
            Serial.println("[ERROR] Wrong values!");
        }
    #endif

    delay(1000);
}
