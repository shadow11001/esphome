#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace lilygo_t5_47_battery {

// Lilygot547Battery is a PollingComponent that reads and reports battery voltage
// for the LilyGO T5 4.7" e-paper device using an ADC sensor. It provides methods
// to receive battery voltage readings.

class Lilygot547Battery : public PollingComponent {
 public:
  // Pointer to the sensor that will report the battery voltage.
  sensor::Sensor *voltage{nullptr};
  // ADC reference voltage in millivolts, typically 1100-1200 for ESP32; adjust for accurate battery readings.
  int vref = 1100;
  // Returns the setup priority for this component.
  // A lower value means higher priority; typically used to control initialization order.
  float get_setup_priority() const override;
  void setup() override;
  // Periodically updates the battery voltage sensor with the latest reading; called at the polling interval.
  void update() override;
  void update_battery_info();
  // Adjusts the ADC reference voltage (vref) for more accurate battery readings.
  // Should be called during setup or when calibration is needed.
  void correct_adc_reference();

  /**
   * @brief Set the sensor to report battery voltage.
   *
   * This method assigns the provided sensor pointer to be used for battery voltage readings.
   * @param voltage_sensor Pointer to a sensor::Sensor instance that will receive battery voltage updates.
   */
  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage = voltage_sensor; }
};

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
