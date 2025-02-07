#include <Arduino.h>
#include "headers/pins.h"
#include "controllers/LedController.h"
#include "controllers/TemperatureController.h"
#include "tasks/MeasuringTemperature.h"
#include "tasks/MonitoringTemperature.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Wi-Fi credentials
const char* ssid = "OnePlus 11 5G";
const char* password = "alfabeta";

LedController* ledController;
TemperatureController* tempController;
TaskHandle_t MeasuringTask;
TaskHandle_t MonitoringTask;
bool isNetworkConnected;
float frequency = 1;

// MQTT broker details
const char* mqtt_server = "34.154.239.184";
const int mqtt_port = 1883;
const char* topic_frequency = "frequency";
const char* topic_samples = "temperature";

WiFiClient espClient;
PubSubClient client(espClient);

/* MQTT subscribing callback */

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(String("Message arrived on [") + topic + "]: ");
  // Converti il payload in una stringa
  String jsonMessage;
  for (unsigned int i = 0; i < length; i++) {
    jsonMessage += (char)payload[i];
  }
  Serial.println("Payload ricevuto: " + jsonMessage);

  // Parsing del JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonMessage);
  if (error) {
    Serial.print("Errore nel parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }

  frequency = doc["frequencyMultiplier"];

  Serial.print("frequency: ");
  Serial.println(frequency);
}

void setup_wifi();
void connect_to_mqtt();

void setup() {
  Serial.begin(115200); 
  ledController = new LedController(new Led(GREEN_LED_PIN), new Led(RED_LED_PIN));
  tempController = new TemperatureController(new TemperatureSensor(TEMP_PIN));

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  xTaskCreatePinnedToCore(measuringTemperatureTask, "measureTemperatureTask", 10000, NULL, 1, &MeasuringTask, 0);
  xTaskCreatePinnedToCore(monitoringTask, "monitoringTask", 10000, NULL, 1, &MonitoringTask, 1);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    isNetworkConnected = false;
    setup_wifi();
  }
  if (!client.connected()) {
    connect_to_mqtt();
  }
  client.loop();
  delay(5000);
}

void setup_wifi() {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected to Wi-Fi");
    isNetworkConnected = true;
}

void connect_to_mqtt() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT broker...");
        if (client.connect("ESP32Client")) {
            Serial.println("Connected");
            client.subscribe(topic_frequency);
        } else {
            Serial.print("Failed. State: ");
            Serial.println(client.state());
            delay(5000);
        }
    }
}
