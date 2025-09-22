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
  if (adc_handle == nullptr) {
    ESP_LOGE(TAG, "ADC not initialized");
    return;
  }

  int raw_value = 0;
  esp_err_t ret = adc_oneshot_read(adc_handle, ADC_CHANNEL_0, &raw_value); // GPIO36 = ADC1_CHANNEL_0

  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to read ADC: %s", esp_err_to_name(ret));
    return;
  }

  double_t battery_voltage;

  if (adc_cali_handle != nullptr) {
    // Use calibrated reading
    int voltage_mv = 0;
    ret = adc_cali_raw_to_voltage(adc_cali_handle, raw_value, &voltage_mv);
    if (ret == ESP_OK) {
      battery_voltage = (voltage_mv / 1000.0) * 2.0; // voltage divider factor
    } else {
      ESP_LOGW(TAG, "Calibration failed, using raw calculation");
      battery_voltage = ((double_t) raw_value / 4095.0) * 2.0 * 3.3 * (this->vref / 1000.0);
    }
  } else {
    // Fallback to raw calculation
    battery_voltage = ((double_t) raw_value / 4095.0) * 2.0 * 3.3 * (this->vref / 1000.0);
  }

  ESP_LOGD(TAG, "ADC raw: %d, Battery voltage: %.3fV", raw_value, battery_voltage);

  if (this->voltage != nullptr) {
    this->voltage->publish_state(battery_voltage);
  }
}

void Lilygot547Battery::correct_adc_reference() {
  // Use new ADC calibration API with line fitting
  adc_cali_line_fitting_config_t cali_config = {
      .unit_id = ADC_UNIT_1,
      .atten = ADC_ATTEN_DB_12,
      .bitwidth = ADC_BITWIDTH_12,
  };


  esp_err_t ret = adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle);
  if (ret == ESP_OK) {
    ESP_LOGD(TAG, "ADC calibration initialized successfully");
  } else {
    ESP_LOGW(TAG, "ADC calibration failed, will use default vref: %s", esp_err_to_name(ret));
    this->vref = 1100; // Use default vref if calibration is not available
    adc_cali_handle = nullptr;
  }
}

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
