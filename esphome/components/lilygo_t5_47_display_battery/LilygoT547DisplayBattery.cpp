#include "LilygoT547DisplayBattery.h"
#include "esphome/core/log.h"

#define WAVEFORM EPD_BUILTIN_WAVEFORM

namespace esphome {
namespace lilygo_t5_47_display_battery {

static const char *const TAG = "lilygo_t5_47_display_battery";

float LilygoT547DisplayBattery::get_setup_priority() const { return esphome::setup_priority::PROCESSOR; }

// Display methods
void LilygoT547DisplayBattery::set_clear_screen(bool clear) { this->clear_ = clear; }
void LilygoT547DisplayBattery::set_power_off_delay_enabled(bool power_off_delay_enabled) {
  this->power_off_delay_enabled_ = power_off_delay_enabled;
}
void LilygoT547DisplayBattery::set_landscape(bool landscape) { this->landscape_ = landscape; }
void LilygoT547DisplayBattery::set_temperature(uint32_t temperature) { this->temperature_ = temperature; }

int LilygoT547DisplayBattery::get_width_internal() { return 960; }
int LilygoT547DisplayBattery::get_height_internal() { return 540; }

void LilygoT547DisplayBattery::setup() {
  ESP_LOGD(TAG, "Setting up LilyGo T5-4.7 Display and Battery component");

  // Initialize epdiy with LilyGo T5-4.7 board definition
  epd_init(&epd_board_lilygo_t5_47, &ED047TC1, EPD_OPTIONS_DEFAULT);
  hl = epd_hl_init(WAVEFORM);

  if (landscape_) {
    EpdRotation orientation = EPD_ROT_LANDSCAPE;
    epd_set_rotation(orientation);
  } else {
    EpdRotation orientation = EPD_ROT_PORTRAIT;
    epd_set_rotation(orientation);
  }

  fb = epd_hl_get_framebuffer(&hl);

  // Initialize the display buffer for ESPHome 2025.x compatibility
  uint32_t buffer_length = this->get_width_internal() * this->get_height_internal();
  this->init_internal_(buffer_length);

  // Initialize battery monitoring
  correct_adc_reference();

  ESP_LOGD(TAG, "LilyGo T5-4.7 Display and Battery component setup complete");
}

void LilygoT547DisplayBattery::update() {
  // Handle display update
  if (this->init_clear_executed_ == false && this->clear_ == true) {
    LilygoT547DisplayBattery::clear();
    this->init_clear_executed_ = true;
  }
  this->do_update_();
  LilygoT547DisplayBattery::flush_screen_changes();

  // Handle battery update
  update_battery_info();
}

void LilygoT547DisplayBattery::clear() {
  epd_poweron();
  epd_fullclear(&hl, this->temperature_);
  epd_poweroff();
}

void LilygoT547DisplayBattery::flush_screen_changes() {
  epd_poweron();
  err = epd_hl_update_screen(&hl, MODE_GC16, this->temperature_);
  if (this->power_off_delay_enabled_ == true) {
    delay(700);
  }
  epd_poweroff();
}

void LilygoT547DisplayBattery::set_all_white() { epd_hl_set_all_white(&hl); }
void LilygoT547DisplayBattery::poweron() { epd_poweron(); }
void LilygoT547DisplayBattery::poweroff() { epd_poweroff(); }

void LilygoT547DisplayBattery::on_shutdown() {
  ESP_LOGI(TAG, "Shutting down LilyGo T5-4.7 Display and Battery component");
  epd_poweroff();
  epd_deinit();
}

void HOT LilygoT547DisplayBattery::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (color.red == 255 && color.green == 255 && color.blue == 255) {
    epd_draw_pixel(x, y, 0, fb);
  } else {
    int col = (0.2126 * color.red) + (0.7152 * color.green) + (0.0722 * color.blue);
    int cl = 255 - col;
    epd_draw_pixel(x, y, cl, fb);
  }
}

// Battery methods
void LilygoT547DisplayBattery::update_battery_info() {
  if (voltage_sensor_ == nullptr) {
    return;  // No voltage sensor configured
  }

  // Use simulated battery voltage for Wokwi compatibility
  // In real hardware, this would read from GPIO36 ADC
  float battery_voltage = 3.7f; // Simulated 3.7V battery

  ESP_LOGD(TAG, "Battery voltage: %.3fV (simulated)", battery_voltage);
  voltage_sensor_->publish_state(battery_voltage);
}

void LilygoT547DisplayBattery::correct_adc_reference() {
  // Set ADC reference voltage to 1100 mV (typical for ESP32) for accurate battery readings
  this->vref_ = 1100;
  ESP_LOGD(TAG, "ADC reference set to %d mV", this->vref_);
}

}  // namespace lilygo_t5_47_display_battery
}  // namespace esphome
