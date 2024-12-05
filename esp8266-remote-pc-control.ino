#include <ESP8266WebServer.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <LittleFS.h>
#include "config.h"

ESP8266WebServer server(80);

WiFiManager wifiManager;

void setup() {
  Serial.begin(BAUD_RATE);

  pinMode(AP_TRIGGER_PIN, INPUT_PULLUP);
  pinMode(STATUS_PIN, INPUT);
  pinMode(POWER_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);

  if (!FILESYSTEM.begin()) {
    Serial.println("[Storage] Couldn't mount file system.");
    return;
  }

  wifiManager.setConfigPortalTimeout(AP_TIMEOUT);
  wifiManager.autoConnect(AP_SSID, AP_PASS);

  initServer();
}

void loop() {
  wifiOnDemandButton();
  server.handleClient();
}

void wifiOnDemandButton() {
  if (digitalRead(AP_TRIGGER_PIN) == LOW) {
    wifiOnDemand();
  }
}

void wifiOnDemand() {
  Serial.println("AP on Demand");
  server.stop();
  if (!wifiManager.startConfigPortal(AP_SSID, AP_PASS)) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
    delay(5000);
  }
  Serial.println("Connected to WiFi.");
  initServer();
}
