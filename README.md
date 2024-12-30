# ESP8266 Power Manager

ESP8266 Power Manager is a lightweight solution to remotely control your PC's power state. This project is particularly useful for systems that lack Wake-on-LAN (WOL) capabilities, enabling users to turn their PC on or off from a distance.

## Features

- Remote power management for PCs using an ESP8266 microcontroller.
- Simple CLI application built with Go and the Cobra library.
- Easy-to-set-up hardware and software integration.
- No need for WOL support in your motherboard.

## How It Works

The ESP8266 microcontroller is connected to the PC's front panel header (power switch). It listens for commands from the CLI application and toggles the PC's power state accordingly.

## Prerequisites

### Hardware Requirements

- An ESP8266 microcontroller (e.g., NodeMCU or similar).
- Basic electronic components for connecting the ESP8266 to the PC's power switch.
- A PC without WOL support (or with WOL disabled).

### Software Requirements

- Go programming language installed on your system.
- A configured ESP8266 firmware to communicate with the CLI application.
- https://github.com/tzapu/WiFiManager

## Installation

### 1. Setting up the ESP8266

1. Flash the ESP8266 with firmware to handle commands (e.g., custom firmware or popular IoT platforms like Tasmota).
2. Connect the ESP8266 to the PC's front panel header using appropriate wiring.
3. Ensure the ESP8266 is connected to your local network.

### 2. Installing the CLI Application

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/powergo.git
   cd powergo
   ```
2. Install the application using `go install`:
   ```bash
   go install ./powergo
   ```
3. Ensure the binary is available in your `PATH`:
   ```bash
   export PATH=$PATH:$(go env GOPATH)/bin
   ```

## Usage

Once installed, use the CLI application to manage your PC's power state. Example usage:

```bash
powergo on    # Turn the PC on
powergo off   # Turn the PC off
```

## Configuration

The CLI app requires the ESP8266's IP address and port for communication. You can set these values using environment variables or configuration files:

- **Environment Variables:**

  ```bash
  export ESP_HOST="192.168.1.x"
  export ESP_PORT="xxxx"
  ```

- **Configuration File:**
  Create a `config.yaml` file in the same directory as the binary:
  ```yaml
  host: 192.168.1.x
  port: xxxx
  ```

## Contributing

Contributions are welcome! If you find a bug or have a feature request, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- ESP8266 community for excellent documentation and firmware support.
- Cobra library for CLI app scaffolding.
