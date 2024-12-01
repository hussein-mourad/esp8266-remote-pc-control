#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include "config.h"

WiFiManager wifiManager;

void setup() {
  Serial.begin(BAUD_RATE);

  wifiManager.autoConnect(AP_SSID, AP_PASS);
}

void loop() {
  wifiOnDemandButton();
  
}

void wifiOnDemandButton() {
  if (digitalRead(AP_TRIGGER_PIN) == LOW) {
    wifiManager.setConfigPortalTimeout(AP_TIMEOUT);
    if (!wifiManager.startConfigPortal(AP_SSID, AP_PASS)) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      ESP.restart();
      delay(5000);
    }
    Serial.println("Connected to WiFi.");
  }
}