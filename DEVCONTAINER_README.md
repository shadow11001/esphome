# ESPHome + Arduino + Wokwi Development Environment

This repository is configured with a devcontainer that provides a complete development environment for ESPHome, Arduino, and Wokwi simulation.

## What's Included

- **ESPHome**: For ESP32/ESP8266 firmware development
- **PlatformIO**: Advanced IDE and build system
- **Arduino CLI**: Command-line Arduino development tools
- **Wokwi CLI**: Virtual hardware simulation
- **Mosquitto**: MQTT broker for testing IoT communications
- **Python 3.11**: With ESPHome and PlatformIO packages
- **Node.js 20**: For any web-based tooling

## Getting Started

1. Open this repository in GitHub Codespaces or VS Code with Dev Containers
2. Wait for the container to build and install all dependencies
3. The following ports will be forwarded:
   - **6052**: ESPHome Dashboard
   - **1883**: Mosquitto MQTT Broker
   - **9011**: Wokwi Web UI

## Quick Start

### Test the Virtual ESP32

1. Open the Wokwi simulation:
   ```bash
   cd wokwi
   wokwi-cli diagram.json
   ```

2. Compile the ESPHome configuration:
   ```bash
   esphome compile esphome/wokwi-esp32.yaml
   ```

3. Start the ESPHome dashboard:
   ```bash
   esphome dashboard config/
   ```

4. Start Mosquitto MQTT broker:
   ```bash
   sudo systemctl start mosquitto
   # Or run in foreground:
   mosquitto -v
   ```

### Files Overview

- `.devcontainer/devcontainer.json`: Development container configuration
- `wokwi/diagram.json`: Virtual ESP32 circuit with DHT22 sensor
- `esphome/wokwi-esp32.yaml`: ESPHome configuration for the virtual device

## Development Workflow

1. Edit the ESPHome configuration in `esphome/wokwi-esp32.yaml`
2. Test compilation with `esphome compile esphome/wokwi-esp32.yaml`
3. Run the simulation with Wokwi to test the virtual hardware
4. Use the ESPHome dashboard to monitor and manage devices
5. Test MQTT communication using the local Mosquitto broker

## VS Code Extensions

The following extensions are automatically installed:
- **PlatformIO IDE**: Complete ESP32/ESP8266 development environment
- **ESPHome**: Syntax highlighting and tools for ESPHome configurations
- **Python**: Full Python development support

## Troubleshooting

- If tools are not found, ensure the postCreateCommand completed successfully
- Check port forwarding is working by accessing http://localhost:6052 for ESPHome
- MQTT broker logs can be viewed with `journalctl -u mosquitto` or by running `mosquitto -v`