#include <ESP8266WebServer.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <LittleFS.h>
#include <ArduinoOTA.h>
#include "config.h"

ESP8266WebServer server(80);

WiFiManager wifiManager;

void setup() {
  Serial.begin(BAUD_RATE);

  ArduinoOTA.setPasswordHash(OTA_HASH);
  ArduinoOTA.onStart(OTAOnStart);
  ArduinoOTA.onEnd(OTAOnEnd);
  ArduinoOTA.onProgress(OTAOnProgress);
  ArduinoOTA.onError(OTAOnError);
  ArduinoOTA.begin();


  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AP_TRIGGER_PIN, INPUT_PULLUP);
  pinMode(STATUS_PIN, INPUT);
  pinMode(POWER_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);  // LED ON

  if (!FILESYSTEM.begin()) {
    Serial.println("[Storage] Couldn't mount file system.");
    return;
  }

  wifiManager.setConfigPortalTimeout(AP_TIMEOUT);
  wifiManager.autoConnect(AP_SSID, AP_PASS);

  Serial.println("Server is starting...");
  initServer();
}

void loop() {
  ArduinoOTA.handle();
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

void OTAOnStart() {
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH) {
    type = "sketch";
  } else {  // U_FS
    type = "filesystem";
  }

  // NOTE: if updating FS this would be the place to unmount FS using FS.end()
  Serial.println("Start updating " + type);
}

void OTAOnEnd() {
  Serial.println("\nEnd");
}

void OTAOnProgress(unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
}

void OTAOnError(ota_error_t error) {
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) {
    Serial.println("Auth Failed");
  } else if (error == OTA_BEGIN_ERROR) {
    Serial.println("Begin Failed");
  } else if (error == OTA_CONNECT_ERROR) {
    Serial.println("Connect Failed");
  } else if (error == OTA_RECEIVE_ERROR) {
    Serial.println("Receive Failed");
  } else if (error == OTA_END_ERROR) {
    Serial.println("End Failed");
  }
}
