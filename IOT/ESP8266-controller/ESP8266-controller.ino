#include <ESP8266WiFi.h>
#include "EspMQTTClient.h"
#include <LiquidCrystal_I2C.h>

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

#define USE_MQTT true
#define USE_LCD true

EspMQTTClient mqttClient(
  WIFI_SSID,
  WIFI_PASS,
  MQTT_BROKER_IP,
  MQTT_USERNAME,
  MQTT_PASSWORD,
  "ESP8266-test",
  MQTT_BROKER_PORT
);

LiquidCrystal_I2C lcd(0x27, 16, 2);

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

 #if (USE_MQTT)
  mqttClient.setMqttReconnectionAttemptDelay(RETRY_BROKER_CONNECTION);
  mqttClient.enableDebuggingMessages();
  mqttClient.enableLastWillMessage("hm/lastwill", "Going offline");
 #endif

 #if (USE_LCD)
  lcd.init();
  lcd.clear();
  lcd.backlight();
  setMessage(lcd, "Booted. Connecting to broker...");
 #endif
}



//  **********************************************************



void loop() {
  #if (USE_MQTT)
   mqttClient.loop();
  #endif
}

// Message should be < 32 char but a longer message will not throw an error
void setMessage(LiquidCrystal_I2C lcd, String message) {
  lcd.clear();
  String line1 = message.substring(0,15);
  String line2 = message.substring(15,31);

  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,2);
  lcd.print(line2);
}

void cleanESP() {
  ESP.eraseConfig();
  WiFi.disconnect();
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(false);
}

void onConnectionEstablished(){
  dbmsg("Connection established");
  setMessage(lcd, "Connected to the broker!");

  mqttClient.subscribe("#", [](const String & payload) {
    dbmsg(payload);
    lightBlink(4, 5);
  });

  mqttClient.subscribe("update/lcd", [](const String & payload) {
    dbmsg(payload);
    setMessage(lcd, payload);
    lightBlink(4, 100);
  });

  mqttClient.subscribe("test/blink", [](const String & payload) {
    lightBlink(12, 25);
  });
}