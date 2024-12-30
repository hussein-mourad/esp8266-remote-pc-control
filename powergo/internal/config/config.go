package config

import (
	"log"
	"os"
	"path/filepath"
	"runtime"

	"gopkg.in/yaml.v3"
)

const (
	AppName = "powergo"
)

type APIConfig struct {
	URL string `yaml:"url"`
}

type WebConfig struct {
	Username string `yaml:"username"`
	Password string `yaml:"password"`
}

type APConfig struct {
	SSID     string `yaml:"ssid"`
	Password string `yaml:"password"`
}

type Config struct {
	API APIConfig `yaml:"api"`
	Web WebConfig `yaml:"web"`
	AP  APConfig  `yaml:"access_point"`
}

func defaultConfig() *Config {
	return &Config{
		API: APIConfig{
			URL: "http://192.168.1.10/api",
		},
		Web: WebConfig{
			Username: "admin",
			Password: "admin",
		},
		AP: APConfig{
			SSID:     "admin",
			Password: "admin",
		},
	}
}

func LoadConfig(filePath string) *Config {
	// Check if the config file exists
	if _, err := os.Stat(filePath); os.IsNotExist(err) {
		log.Println("Config file not found, creating a default one...")

		// Create a default config
		defaultConfig := defaultConfig()

		file, err2 := os.Create(filePath)
		if err2 != nil {
			log.Fatalf("Error creating config file: %v", err2)
		}
		defer file.Close()

		encoder := yaml.NewEncoder(file)
		if err = encoder.Encode(&defaultConfig); err != nil {
			log.Fatalf("Error writing default config to file: %v", err)
		}
	}

	// Load the configuration
	file, err := os.Open(filePath)
	if err != nil {
		log.Fatalf("Error opening config file: %v", err)
	}
	defer file.Close()

	var config Config
	decoder := yaml.NewDecoder(file)
	if err := decoder.Decode(&config); err != nil {
		log.Fatalf("Error decoding config file: %v", err)
	}

	return &config
}

func GetDefaultConfigFilePath() string {
	var configDir string
	switch runtime.GOOS {
	case "windows":
		configDir = os.Getenv("APPDATA")
	case "darwin":
		configDir = filepath.Join(os.Getenv("HOME"), "Library", "Application Support")
	default: // Assume Linux or other Unix-like systems
		configDir = os.Getenv("XDG_CONFIG_HOME")
		if configDir == "" {
			configDir = filepath.Join(os.Getenv("HOME"), ".config")
		}
	}

	if configDir == "" {
		log.Fatalf("unable to determine configuration directory for %s", runtime.GOOS)
	}

	// Ensure the directory exists, create it if it doesn't.
	configDirPath := filepath.Join(configDir, AppName)
	err := os.MkdirAll(configDirPath, os.ModePerm)
	if err != nil {
		log.Fatalf("unable to create configuration directory: %v", err)
	}

	return filepath.Join(configDirPath, "config.yaml")
}

func GetDefaultConfigStr() string {
	defaultConfig := defaultConfig()
	yamlData, err := yaml.Marshal(&defaultConfig)
	if err != nil {
		log.Fatalf("error encoding YAML: %v", err)
	}
	return string(yamlData)
}
