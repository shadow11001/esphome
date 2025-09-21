#pragma once
#include <Arduino.h>
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace lilygo_t5_47_battery {

class Lilygot547Battery : public PollingComponent {
 public:
  sensor::Sensor *voltage{nullptr};

  int vref = 1100;
  float get_setup_priority() const override;
  void setup() override;
  void update() override;
  void update_battery_info();
  void correct_adc_reference();

  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage = voltage_sensor; }
};
}  // namespace lilygo_t5_47_battery
}  // namespace esphome
