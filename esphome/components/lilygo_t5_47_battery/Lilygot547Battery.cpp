#include "Lilygot547Battery.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lilygo_t5_47_battery {

static const char *const TAG = "lilygo_t5_47_battery";

float Lilygot547Battery::get_setup_priority() const { return esphome::setup_priority::DATA; }

void Lilygot547Battery::setup() {
  ESP_LOGD(TAG, "Battery component setup - simulation mode");
}

void Lilygot547Battery::update() {
  // No need for epd_poweron/poweroff in simulation mode
  update_battery_info();
}

void Lilygot547Battery::update_battery_info() {
  // Use simulated battery voltage for Wokwi compatibility
  float battery_voltage = 3.7f; // Simulated 3.7V battery

  ESP_LOGD(TAG, "Simulated battery voltage: %.3fV", battery_voltage);

  if (this->voltage != nullptr) {
    this->voltage->publish_state(battery_voltage);
  }
}

void Lilygot547Battery::correct_adc_reference() {
  // Set ADC reference voltage to 1100 mV (typical for ESP32) for accurate battery readings
  this->vref = 1100;
}

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
