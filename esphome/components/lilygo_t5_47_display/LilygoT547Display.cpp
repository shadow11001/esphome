#include "LilygoT547Display.h"
#include "esphome/core/log.h"

#ifdef USE_EPDIY
#define WAVEFORM EPD_BUILTIN_WAVEFORM
#endif

namespace esphome {
namespace lilygo_t5_47_display {

static const char *const TAG = "lilygo_t5_47_display";

float LilygoT547Display::get_setup_priority() const { return esphome::setup_priority::PROCESSOR; }

void LilygoT547Display::set_clear_screen(bool clear) { this->clear_ = clear; }
void LilygoT547Display::set_power_off_delay_enabled(bool power_off_delay_enabled) {
  this->power_off_delay_enabled_ = power_off_delay_enabled;
}
void LilygoT547Display::set_landscape(bool landscape) { this->landscape_ = landscape; }

void LilygoT547Display::set_temperature(uint32_t temperature) { this->temperature_ = temperature; }

int LilygoT547Display::get_width_internal() { return 960; }

int LilygoT547Display::get_height_internal() { return 540; }

void LilygoT547Display::setup() {
#ifdef USE_EPDIY
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
#else
  // Arduino framework - stub implementation without epdiy
  ESP_LOGW(TAG, "Display functionality disabled - epdiy library not available in Arduino framework");
  uint32_t buffer_length = this->get_width_internal() * this->get_height_internal();
  this->init_internal_(buffer_length);
#endif
}

void LilygoT547Display::update() {
#ifdef USE_EPDIY
  if (this->init_clear_executed_ == false && this->clear_ == true) {
    LilygoT547Display::clear();
    this->init_clear_executed_ = true;
  }
  this->do_update_();
  LilygoT547Display::flush_screen_changes();
#else
  // Arduino framework - stub implementation
  ESP_LOGD(TAG, "Display update skipped - epdiy not available");
#endif
}

void LilygoT547Display::clear() {
#ifdef USE_EPDIY
  epd_poweron();
  epd_fullclear(&hl, this->temperature_);
  epd_poweroff();
#endif
}

void LilygoT547Display::flush_screen_changes() {
#ifdef USE_EPDIY
  epd_poweron();
  err = epd_hl_update_screen(&hl, MODE_GC16, this->temperature_);
  if (this->power_off_delay_enabled_ == true) {
    delay(700);
  }
  epd_poweroff();
#endif
}

void LilygoT547Display::set_all_white() { 
#ifdef USE_EPDIY
  epd_hl_set_all_white(&hl); 
#endif
}
void LilygoT547Display::poweron() { 
#ifdef USE_EPDIY
  epd_poweron(); 
#endif
}
void LilygoT547Display::poweroff() { 
#ifdef USE_EPDIY
  epd_poweroff(); 
#endif
}

void LilygoT547Display::on_shutdown() {
  ESP_LOGI(TAG, "Shutting down Lilygo T5-4.7 screen");
#ifdef USE_EPDIY
  epd_poweroff();
  epd_deinit();
#endif
}

void HOT LilygoT547Display::draw_absolute_pixel_internal(int x, int y, Color color) {
#ifdef USE_EPDIY
  if (color.red == 255 && color.green == 255 && color.blue == 255) {
    epd_draw_pixel(x, y, 0, fb);
  } else {
    int col = (0.2126 * color.red) + (0.7152 * color.green) + (0.0722 * color.blue);
    int cl = 255 - col;
    epd_draw_pixel(x, y, cl, fb);
  }
#else
  // Arduino framework - no pixel drawing without epdiy
#endif
}

}  // namespace lilygo_t5_47_display
}  // namespace esphome
