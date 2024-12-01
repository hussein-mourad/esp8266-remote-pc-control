#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <DNSServer.h>

#define BAUD_RATE 115200
#define HTTP_REST_PORT 8080

DNSServer dnsServer;
ESP8266WebServer server(80);
WiFiManager wifiManager; 

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";

//Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* ipPath = "/ip.txt";
const char* gatewayPath = "/gateway.txt";

IPAddress localIP;
IPAddress localGateway;
IPAddress subnet(255, 255, 0, 0);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

// Set LED GPIO
const int ledPin = 2;
// Stores LED state

String ledState;

boolean restart = false;

// Configuration
const char *apSSID = "ESP8266-Config";       // AP SSID
const char *apPassword = "password";      // AP Password (optional)
IPAddress apIP(192, 168, 4, 1);              // Static IP for AP
IPAddress netMsk(255, 255, 255, 0);          // Subnet Mask

void setup(void) {
  Serial.begin(BAUD_RATE);
  
  // int n = WiFi.scanNetworks();
  // for (int i = 1; i < n; i++) {
  //   String ssid = WiFi.SSID(i);
  //   int rssi = WiFi.RSSI(i);
  //   bool isEncrypted = WiFi.encryptionType(i) != AUTH_OPEN;
  //   Serial.println(ssid);
  //   Serial.println(rssi);
  //   Serial.println(isEncrypted);
  // }
  // Start Access Point
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(apSSID, apPassword);

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS Server to redirect all queries
  dnsServer.start(53, "*", apIP);

  // Redirect any path to the configuration portal
  server.onNotFound([]() {
    String html = buildConfigPortalHTML();
    server.send(200, "text/html", html);
  });

   // Handle WiFi credentials submission
  server.on("/save", HTTP_POST, []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    Serial.println("Received WiFi Credentials:");
    Serial.println("SSID: " + ssid);
    Serial.println("Password: " + password);

    // Save credentials or connect to WiFi
    server.send(200, "text/plain", "Saved! Rebooting...");
    delay(1000);
    ESP.restart(); // Restart to apply changes
  });

  server.begin();

  // wifiManager.autoConnect();
  // wifiManager.setAPCallback(configModeCallback);
  // wifiManager.setClass("invert"); // dark theme

  // initFS();

  // // Load values saved in LittleFS
  // ssid = readFile(LittleFS, ssidPath);
  // pass = readFile(LittleFS, passPath);
  // ip = readFile(LittleFS, ipPath);
  // gateway = readFile(LittleFS, gatewayPath);
  // Serial.println(ssid);
  // Serial.println(pass);
  // Serial.println(ip);
  // Serial.println(gateway)

  // if (initWiFi()) {
  //   // Set server routing
  //   router();
  // } else {
  // }

  // // Wait for connection
  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(WIFI_SSID);
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  // // Set not found response
  // server.onNotFound(handleNotFound);
  // // Start server
  // server.begin();
  // Serial.println("HTTP server started");
}

void loop(void) {
  // delay(1000);
  // wifiManager.startConfigPortal("OnDemandAP");
  dnsServer.processNextRequest();
  server.handleClient();
}


String buildConfigPortalHTML() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Captive Portal</title>
      <style>
        body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }
        input { margin: 10px 0; padding: 10px; width: 80%; }
        button { padding: 10px 20px; }
      </style>
    </head>
    <body>
      <h1>WiFi Configuration</h1>
      <form method="POST" action="/save">
        <input type="text" name="ssid" placeholder="SSID" required><br>
        <input type="password" name="password" placeholder="Password" required><br>
        <button type="submit">Save</button>
      </form>
    </body>
    </html>
  )rawliteral";
  return html;
}

void indexHandler() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(404, F("text/html"), F("Not found"));
    return;
  }

  server.streamFile(file, F("text/html"));
  file.close();
}

// Serving Hello world
void getHelloWord() {
  server.send(200, "application/json", "{\"name\": \"Hello world\"}");
}

// Define routing
void router() {
  server.on("/", HTTP_GET, indexHandler);
  server.on(F("/hello"), HTTP_GET, getHelloWord);
}

// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

// Initialize WiFi
bool initWiFi() {
  if (ssid == "" || ip == "") {
    Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  localIP.fromString(ip.c_str());
  localGateway.fromString(gateway.c_str());

  if (!WiFi.config(localIP, localGateway, subnet)) {
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), pass.c_str());

  Serial.println("Connecting to WiFi...");
  delay(20000);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect.");
    return false;
  }

  Serial.println(WiFi.localIP());
  return true;
}

// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  } else {
    Serial.println("LittleFS mounted successfully");
  }
}

// Read File from LittleFS
String readFile(fs::FS& fs, const char* path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available()) {
    fileContent = file.readStringUntil('\n');
    break;
  }
  file.close();
  return fileContent;
}

// Write file to LittleFS
void writeFile(fs::FS& fs, const char* path, const char* message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- frite failed");
  }
  file.close();
}
