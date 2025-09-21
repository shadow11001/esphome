#include "Lilygot547Battery.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lilygo_t5_47_battery {

static const char *const TAG = "lilygo_t5_47_battery";

float Lilygot547Battery::get_setup_priority() const { return esphome::setup_priority::DATA; }

void Lilygot547Battery::setup() {}

void Lilygot547Battery::update() {
  // Simple battery reading using Arduino ADC - no driver conflicts
  Lilygot547Battery::update_battery_info();
}
void Lilygot547Battery::update_battery_info() {
  // Use Arduino's analogRead() to avoid ADC driver conflicts with epdiy
  // GPIO36 is the battery voltage divider input
  uint16_t v = analogRead(36);
  double_t battery_voltage = ((double_t) v / 4095.0) * 2.0 * 3.3 * (this->vref / 1000.0);
  
  if (this->voltage != nullptr) {
    this->voltage->publish_state(battery_voltage);
  }
}

void Lilygot547Battery::correct_adc_reference() {
  // Use default vref for Arduino analogRead() approach
  this->vref = 1100;
}

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
