#include <ESP8266WiFi.h>
#include "EspMQTTClient.h"

// Custom libraries
#include "credentials.h"
#include "builtinLed.h"

/*
  DEBUGGING comes with level.
  No conventions, feel free to update.
  1 - Custom debug messages only
  2 - Custom and lib debu messages
*/
#define DEBUGGING 1 // Up the debug mode

#define RETRY_BROKER_CONNECTION 5000 // ms

EspMQTTClient mqttClient(
  WIFI_SSID,
  WIFI_PASS,
  MQTT_BROKER_IP,
  MQTT_USERNAME,
  MQTT_PASSWORD,
  "ESP8266-test",
  MQTT_BROKER_PORT
);

void setup() {
  delay(100);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();

  #if DEBUGGING == 2
  //  set debug for core/ESP
    Serial.setDebugOutput(true);
  #else
    Serial.setDebugOutput(false);
  #endif

  mqttClient.setMqttReconnectionAttemptDelay(RETRY_BROKER_CONNECTION);
  mqttClient.enableDebuggingMessages();
  mqttClient.enableLastWillMessage("hm/lastwill", "Going offline");
}



//  **********************************************************



void loop() {
 mqttClient.loop();
}

void cleanESP() {
  ESP.eraseConfig();
  WiFi.disconnect();
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(false);
}

void onConnectionEstablished(){
  dbmsg("Connection established");

  mqttClient.subscribe("*", [](const String & payload) {
    dbmsg(payload);
    lightBlink(10, 150);
  });
}