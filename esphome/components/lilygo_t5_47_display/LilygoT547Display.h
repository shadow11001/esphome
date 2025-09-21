#pragma once
#include "esphome/core/component.h"

#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"
#include "esphome/core/hal.h"
#include "esphome/core/version.h"

#ifdef USE_EPDIY
#ifndef EPD_DRIVER
#define EPD_DRIVER
#include "epdiy.h"
#endif
#endif

namespace esphome {
namespace lilygo_t5_47_display {

// LilyGo-EPD47
class LilygoT547Display : public display::DisplayBuffer {
 protected:
#ifdef USE_EPDIY
  EpdiyHighlevelState hl;
  // ambient temperature around device
  uint8_t *fb;
  enum EpdDrawError err;
#endif

 public:
  float get_setup_priority() const override;

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

 protected:
  void HOT draw_absolute_pixel_internal(int x, int y, Color color) override;

  bool clear_;
  bool init_clear_executed_ = false;
  bool temperature_;
  bool power_off_delay_enabled_;
  bool landscape_;
};

}  // namespace lilygo_t5_47_display
}  // namespace esphome
