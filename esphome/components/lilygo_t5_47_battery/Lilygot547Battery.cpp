#include "Lilygot547Battery.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lilygo_t5_47_battery {

static const char *const TAG = "lilygo_t5_47_battery";

float Lilygot547Battery::get_setup_priority() const { return esphome::setup_priority::DATA; }

void Lilygot547Battery::setup() {}

void Lilygot547Battery::update() {
  // No EPD power control needed - battery reading doesn't require EPD to be powered
  Lilygot547Battery::update_battery_info();
}
void Lilygot547Battery::update_battery_info() {
  Lilygot547Battery::correct_adc_reference();
  // 36 main power supply ?
  // 35 battery ?
  
  // Use ESP-IDF NG ADC (driver_ng) for all frameworks to ensure consistency
  adc_oneshot_unit_handle_t adc_handle;
  adc_oneshot_unit_init_cfg_t init_config = {
      .unit_id = ADC_UNIT_1,
      .ulp_mode = ADC_ULP_MODE_DISABLE,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

  adc_oneshot_chan_cfg_t config = {
      .atten = ADC_ATTEN_DB_12,
      .bitwidth = ADC_BITWIDTH_12,
  };
  ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_0, &config));

  int adc_raw;
  ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_0, &adc_raw));
  ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_handle));

  double_t battery_voltage = ((double_t) adc_raw / 4095.0) * 2.0 * 3.3 * (this->vref / 1000.0);
  
  if (this->voltage != nullptr) {
    this->voltage->publish_state(battery_voltage);
  }
}

void Lilygot547Battery::correct_adc_reference() {
  // Use ESP-IDF NG ADC calibration for all frameworks
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
  } else {
    // Fallback to default vref
    this->vref = 1100;
  }
}

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
