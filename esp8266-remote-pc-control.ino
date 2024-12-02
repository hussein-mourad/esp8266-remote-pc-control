#include <Arduino.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266WebServer.h>
#include "config.h"
#include <LittleFS.h>

ESP8266WebServer server(80);

void homeRequest() {
  server.send(200, "text/plain", "Hello, world");
}

void notFound() {
  if (!handleFileRead(server.uri())) {
    Serial.println("[Storage] Couldn't find file at \'" + server.uri() + "\'" + ".");
    server.send(404, "text/plain", "Not found");
  }
}

void InitServer() {
  server.on("/", HTTP_GET, homeRequest);
  server.onNotFound(notFound);

  server.begin();
  Serial.println("HTTP server started");
}

WiFiManager wifiManager;

void setup() {
  Serial.begin(BAUD_RATE);

  if (!FILESYSTEM.begin()) {
    Serial.println("[Storage] Couldn't mount file system.");
    return;
  }

  wifiManager.autoConnect(AP_SSID, AP_PASS);

  InitServer();
}

void loop() {
  wifiOnDemandButton();
  server.handleClient();
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

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {
  Serial.println("[Storage] File read: " + path);

  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);

  if (FILESYSTEM.exists(path)) {
    File file = FILESYSTEM.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}
