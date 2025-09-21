#pragma once
#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"
#include "esphome/core/version.h"

#ifndef EPD_DRIVER
#define EPD_DRIVER
#include "epdiy.h"
#endif

namespace esphome {
namespace lilygo_t5_47_display_battery {

// Combined LilyGo T5-4.7 Display and Battery component
// Handles both e-paper display and battery voltage monitoring in a single component
// to avoid ADC driver conflicts in simulation environments
class LilygoT547DisplayBattery : public display::DisplayBuffer {
 protected:
  // Display related members
  EpdiyHighlevelState hl;
  uint8_t *fb;
  enum EpdDrawError err;
  bool clear_;
  bool init_clear_executed_ = false;
  bool temperature_;
  bool power_off_delay_enabled_;
  bool landscape_;

  // Battery related members
  sensor::Sensor *voltage_sensor_{nullptr};
  int vref_ = 1100;  // ADC reference voltage in millivolts

 public:
  float get_setup_priority() const override;

  // Display methods
  void set_clear_screen(bool clear);
  void set_landscape(bool landscape);
  void set_power_off_delay_enabled(bool power_off_delay_enabled);
  void set_temperature(uint32_t temperature);

  int get_width_internal() override;
  int get_height_internal() override;

  void setup() override;
  void update() override;

  void clear();
  void flush_screen_changes();
  void set_all_white();
  void poweron();
  void poweroff();
  void on_shutdown() override;

#if ESPHOME_VERSION_CODE >= VERSION_CODE(2025,9,0)
  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
#endif

  // Battery methods
  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
  void update_battery_info();
  void correct_adc_reference();

 protected:
  void HOT draw_absolute_pixel_internal(int x, int y, Color color) override;
};

}  // namespace lilygo_t5_47_display_battery
}  // namespace esphome
