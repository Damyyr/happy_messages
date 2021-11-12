#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <UPnP_Generic.h>
#include "credentials.h"

#define LEASE_DURATION 3600
#define TIMEOUT 30000

#define DEBUGGING // Up the debug mode
#define _UPNP_LOGLEVEL_ 3

#define DEBUG_ESP_HTTP_SERVER 1
#define DEBUG_ESP_PORT 1

// DDNS configuration
#define DDNS_USING_WIFI             true
#define DDNS_USING_ETHERNET         false

UPnP* uPnP;

#define MAX_UPNP_RETRIES 5

ESP8266WebServer webserver(PORT);
//WiFiServer webserver(PORT);

void setup() {
  delay(10);
  Serial.begin(115200);
  Serial.println();

#ifdef DEBUGGING
//  set debug for core/ESP
  Serial.setDebugOutput(true);
#endif

// Force webserver configurations
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.config(ip, gateway, subnet);

// Connect to Wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

// Print Wifi informations
  Serial.println();
  Serial.print("Connected! IP: ");
  IPAddress actualIp = WiFi.localIP();
  Serial.print(actualIp);
  Serial.println();


// Routing Begins ------------------------
  webserver.on("/", rootPage);
  webserver.on("/lightUp", lightUp);
  webserver.on("/lightDown", lightDown);
  webserver.onNotFound(notFoundPage);
// Routing Ends --------------------------


// Starts server
  webserver.begin();

  pinMode(LED_BUILTIN, OUTPUT);

// Print server informations
  Serial.print("Listening on port ");
  Serial.print(PORT);
  Serial.println();

// Setup UPnP for port forwarding

int retries = 0;
bool portMappingAdded = false;

  while (!portMappingAdded && (retries < MAX_UPNP_RETRIES))
  {
    Serial.println("Forwarding port, try # " + String(++retries));

    Serial.print("Configuration... ");
    uPnP = new UPnP(PORT);
    uPnP->addPortMappingConfig(actualIp, PORT, RULE_PROTOCOL_TCP, LEASE_DURATION, "StupidDevESP");
    int result = uPnP->commitPortMappings();

    // Serial.println("Open ports with response: ");
    // Serial.print(result);

    portMappingAdded = ((result == PORT_MAP_SUCCESS) || (result == ALREADY_MAPPED));

    if(portMappingAdded){
      Serial.print("SUCCESS: Port has been opened");
    } else {
      Serial.print("ERROR: Port can't be open");
      uPnP->printAllPortMappings();
      delay(2000);
    }

    Serial.println();
  }

  if(!portMappingAdded)
    Serial.println("Failed to open a port");

  uPnP->printPortMappingConfig();
  uPnP->printAllPortMappings();
}



//  **********************************************************



void loop() {
  // put your main code here, to run repeatedly:
  webserver.handleClient();

//  applyLightSettings();
}

void cleanESP() {
  ESP.eraseConfig();
  WiFi.disconnect();
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(false);
}

void lightUp(){
  dbmsg("light up");
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  send_OK();
}

void lightDown(){
  dbmsg("light down");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  send_OK();
}

void send_OK(){
  webserver.send(200, "text/plain", "OK");
  dbmsg("OK sent");
}

void dbmsg(String message){
  #ifdef DEBUGGING
    Serial.println("DEBUG: " + message);
  #endif
}

void rootPage() {
  dbmsg("root landed");
  webserver.send(200, "text/plain", "Hello world!");
}

void notFoundPage() {
  dbmsg("not found");
  Serial.println("Someone just poked me");
  webserver.send(404, "text/plain", "Not Found");
}