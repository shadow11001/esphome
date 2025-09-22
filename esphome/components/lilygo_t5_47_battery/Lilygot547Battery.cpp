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
  Lilygot547Battery::correct_adc_reference();
  // 36 main power supply ?
  // 35 battery ?
  uint16_t v = analogRead(36);
  double_t battery_voltage = ((double_t) v / 4095.0) * 2.0 * 3.3 * (this->vref / 1000.0);
  if (this->voltage != nullptr) {
    this->voltage->publish_state(battery_voltage);
  }
}

void Lilygot547Battery::correct_adc_reference() {
  // Use new ADC calibration API with line fitting
  adc_cali_handle_t adc_cali_handle = nullptr;
  adc_cali_line_fitting_config_t cali_config = {
      .unit_id = ADC_UNIT_1,
      .atten = ADC_ATTEN_DB_12,
      .bitwidth = ADC_BITWIDTH_12,
  };

  esp_err_t ret = adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle);
  if (ret == ESP_OK && adc_cali_handle != nullptr) {
    // Use default vref if calibration is not available
    this->vref = 1100;
    adc_cali_delete_scheme_line_fitting(adc_cali_handle);
  }
}

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
