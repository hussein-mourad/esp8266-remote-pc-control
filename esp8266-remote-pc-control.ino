#include <ESP8266WebServer.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <LittleFS.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include "config.h"

ESP8266WebServer server(80);

struct Config {
  String webUsername;
  String webPassword;
  String apSSID = AP_SSID;
  String apPass;
};

Config config;

WiFiManager wifiManager;
WiFiManagerParameter webUsernameField("webUsername", "Web Username", config.webUsername.c_str(), 64);
WiFiManagerParameter webPasswordField("webPassword", "Web Password", config.webPassword.c_str(), 64);
WiFiManagerParameter apSSIDField("apSSID", "Access Point SSID", config.apSSID.c_str(), 64);
WiFiManagerParameter apPassField("apPass", "Access Point Password", config.apPass.c_str(), 64);
// WiFiManagerParameter apPassField("apPass", "Access Point Password", config.apSSID, 64, "type='password'");

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

  // wifiManager.resetSettings();        // For debugging only
  // FILESYSTEM.remove("/config.json");  // Remove stored parameters

  loadConfig();

  // if (config.webUsername == "") {
  //   wifiManager.resetSettings();        // Reset WiFi credentials
  //   FILESYSTEM.remove("/config.json");  // Remove stored parameters
  //   ESP.restart();                      // Restart device
  // }

  webUsernameField.setValue(config.webUsername.c_str(), 64);
  webPasswordField.setValue(config.webPassword.c_str(), 64);
  apSSIDField.setValue(config.apSSID.c_str(), 64);
  apPassField.setValue(config.apPass.c_str(), 64);

  wifiManager.addParameter(&webUsernameField);
  wifiManager.addParameter(&webPasswordField);
  wifiManager.addParameter(&apSSIDField);
  wifiManager.addParameter(&apPassField);

  // if (!strcmp(config.apPass.c_str(), "")) {
  //   Serial.println("Pass is not set");
  // } else {
  //   Serial.println("Pass is set.....");
  //   Serial.println(config.apPass);
  // }

  wifiManager.autoConnect(config.apSSID.c_str(), config.apPass.c_str());
  wifiManager.setSaveConfigCallback(saveConfigCallback);

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
    wifiManager.resetSettings();        // Reset WiFi credentials
    FILESYSTEM.remove("/config.json");  // Remove stored parameters
    ESP.restart();                      // Restart device
  }
}

void wifiOnDemand() {
  Serial.println("AP on Demand");
  server.stop();
  if (!wifiManager.startConfigPortal(config.apSSID.c_str(), config.apPass.c_str())) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
    delay(5000);
  }
  Serial.println("Connected to WiFi.");
  initServer();
}

void loadConfig() {
  if (FILESYSTEM.exists("/config.json")) {
    File configFile = FILESYSTEM.open("/config.json", "r");
    if (configFile) {
      StaticJsonDocument<256> jsonDoc;
      DeserializationError error = deserializeJson(jsonDoc, configFile);
      if (!error) {
        config.webUsername = jsonDoc["web_username"].as<String>();
        config.webPassword = jsonDoc["web_password"].as<String>();
        config.apSSID = jsonDoc["access_point_ssid"].as<String>();
        config.apPass = jsonDoc["access_point_password"].as<String>();

        Serial.println("Loaded config.");
        Serial.println("Username: " + config.webUsername);
        Serial.println("Password: " + config.webPassword);
        Serial.println("SSID: " + config.apSSID);
        Serial.println("Password: " + config.apPass);
      }
      configFile.close();
    } else {
      Serial.println("Failed to open config file!");
    }
  } else {
    Serial.println("Config file not found!");
  }
}

void saveConfig(Config cfg) {
  // Save to LittleFS
  StaticJsonDocument<256> jsonDoc;
  jsonDoc["web_username"] = cfg.webUsername;
  jsonDoc["web_password"] = cfg.webPassword;
  jsonDoc["access_point_ssid"] = cfg.apSSID;
  jsonDoc["access_point_password"] = cfg.apPass;

  File configFile = FILESYSTEM.open("/config.json", "w");
  if (configFile) {
    serializeJson(jsonDoc, configFile);
    configFile.close();
    Serial.println("Saved config.");
  } else {
    Serial.println("Failed to save config file!");
  }
}

void saveConfigCallback() {
  Serial.println("Should save config");
  Serial.printf("WWW Username %s\n", config.webUsername.c_str());
  Serial.printf("WWW Pass %s\n", config.webPassword.c_str());
  Serial.printf("AP SSID %s\n", config.apSSID.c_str());
  Serial.printf("AP Pass %s\n", config.apPass.c_str());

  config.webUsername = webUsernameField.getValue();
  config.webPassword = webPasswordField.getValue();
  config.apSSID = apSSIDField.getValue();
  config.apPass = apPassField.getValue();

  saveConfig(config);

  Serial.printf("WWW Username %s\n", config.webUsername.c_str());
  Serial.printf("WWW Pass %s\n", config.webPassword.c_str());
  Serial.printf("AP SSID %s\n", config.apSSID.c_str());
  Serial.printf("AP Pass %s\n", config.apPass.c_str());
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
