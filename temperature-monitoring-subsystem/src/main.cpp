#include <Arduino.h>
#include "headers/pins.h"
#include "controllers/LedController.h"
#include "controllers/TemperatureController.h"
#include "tasks/MeasuringTemperature.h"
#include "tasks/MonitoringTemperature.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "utils/Connection.h"

// Wi-Fi credentials
const char* ssid = "TIM-43496337";
const char* password = "DSE5udhet7tk6KAd3QsZAHQh";

LedController* ledController;
TemperatureController* tempController;
TaskHandle_t MeasuringTask;
TaskHandle_t MonitoringTask;
bool isNetworkConnected;

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
  DynamicJsonDocument doc(200);
  DeserializationError error = deserializeJson(doc, jsonMessage);

  if (error) {
    Serial.print("Errore nel parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Leggi i valori dal JSON
  float frequency = doc[topic_frequency]; 

  // Stampa i valori
  Serial.print("frequency: ");
  Serial.println(frequency);
}

void setup() {
  Serial.begin(115200); 
  ledController = new LedController(new Led(GREEN_LED_PIN), new Led(RED_LED_PIN));
  tempController = new TemperatureController(new TemperatureSensor(TEMP_PIN));

  connect_wifi((char*) ssid, (char*) password);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  xTaskCreatePinnedToCore(measuringTemperatureTask, "measureTemperatureTask", 10000, NULL, 1, &MeasuringTask, 0);
  xTaskCreatePinnedToCore(monitoringTask, "monitoringTask", 10000, NULL, 1, &MonitoringTask, 1);
}

void loop() {
  if (!client.connected()) {
    isNetworkConnected = false;
    connect_to_mqtt(client);
    client.subscribe(topic_frequency);
  }
  client.loop();
  delay(5000);
}
