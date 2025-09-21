#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

// Use ESP-IDF NG ADC driver for both Arduino and ESP-IDF frameworks
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#ifdef USE_ESP_IDF
#ifndef EPD_DRIVER
#define EPD_DRIVER
#include "epdiy.h"
#endif
#endif

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
