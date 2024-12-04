void initServer() {
  server.on("/api/power/status", handlePowerStatus);
  server.on("/api/power/on", handlePowerOn);
  server.on("/api/power/off", handlePowerOff);
  server.on("/api/power/forceoff", handleForceOff);
  server.on("/api/power/reboot", handleReboot);
  server.on("/api/esp/reboot", handleESPReboot);
  server.on("/api/wifi/config", handleWifiConfig);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void handlePowerStatus() {
  if (!authenticate()) return;
  if (digitalRead(STATUS_PIN) == STATUS_ON) {
    server.send(200, "text/plain", "Up");
  } else {
    server.send(200, "text/plain", "Down");
  }
}

void handlePowerOn() {
  if (!authenticate()) return;
  if (digitalRead(STATUS_PIN) == STATUS_ON) {
    server.send(400, "text/plain", "PC is already on.");
    return;
  }
  server.send(200, "text/plain", "PC is powering on...");
  digitalWrite(POWER_PIN, POWER_SIGNAL);
  delay(POWER_DELAY);
  digitalWrite(POWER_PIN, !POWER_SIGNAL);
}

void handlePowerOff() {
  if (!authenticate()) return;
  if (digitalRead(STATUS_PIN) != STATUS_ON) {
    server.send(400, "text/plain", "PC is already off.");
    return;
  }
  server.send(200, "text/plain", "PC is powering off...");
  digitalWrite(POWER_PIN, POWER_SIGNAL);
  delay(POWER_DELAY);
  digitalWrite(POWER_PIN, !POWER_SIGNAL);
}

void handleForceOff() {
  if (!authenticate()) return;
  if (digitalRead(STATUS_PIN) != STATUS_ON) {
    server.send(400, "text/plain", "PC is already off.");
    return;
  }
  server.send(200, "text/plain", "PC is powering off...");
  delay(500);
  digitalWrite(POWER_PIN, POWER_SIGNAL);
  delay(FORCE_OFF_DELAY);
  digitalWrite(POWER_PIN, !POWER_SIGNAL);
}

void handleReboot() {
  if (!authenticate()) return;
  if (digitalRead(STATUS_PIN) != STATUS_ON) {
    server.send(400, "text/plain", "PC is off.");
    return;
  }
  server.send(200, "text/plain", "PC is rebooting...");
  digitalWrite(RESET_PIN, RESET_SIGNAL);
  delay(POWER_DELAY);
  digitalWrite(RESET_PIN, !RESET_SIGNAL);
}

void handleESPReboot() {
  if (!authenticate()) return;
  server.send(200, "text/plain", "System is restarting...");
  delay(500);
  ESP.restart();
}

void handleWifiConfig() {
  if (!authenticate()) return;
  server.send(200, "text/plain", "Connect to AP and Go to 192.168.4.1");
  delay(500);
  wifiOnDemand();
}

void handleNotFound() {
  if (!authenticate()) return;
  if (!handleFileRead(server.uri())) {
    Serial.println("[Storage] Couldn't find file at \'" + server.uri() + "\'" + ".");
    server.send(404, "text/plain", "Not found");
  }
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

bool authenticate() {
  if (!server.authenticate(WWW_USERNAME, WWW_PASSWORD)) {
    server.requestAuthentication();
    return false;  // Stop further processing
  }
  return true;  // Proceed to the route handler
}