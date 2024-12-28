#ifndef CONFIG
#define BAUD_RATE 115200

#define OTA_HASH "<your-md5-hash>" // printf 'your-password' | md5sum | cut -d ' ' -f1 | tr -d ' '

#define WWW_USERNAME "<your-username>"
#define WWW_PASSWORD "<your-password>"

#define AP_SSID "<your-ssid>"
#define AP_PASS "<your-password>"
#define AP_TIMEOUT 180    // 3 minutes
#define AP_TRIGGER_PIN 0  // Flash button

#define FILESYSTEM LittleFS

#define STATUS_PIN D5
#define POWER_PIN D1
#define RESET_PIN D2

#define STATUS_ON 1
#define POWER_SIGNAL 1
#define RESET_SIGNAL 1

#define POWER_DELAY 200
#define FORCE_OFF_DELAY 4000
#endif