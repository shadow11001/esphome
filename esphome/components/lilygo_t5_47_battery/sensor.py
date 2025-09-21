import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_VOLTAGE,
    UNIT_VOLT,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ["esp32"]

from . import lilygo_t5_47_battery_ns

Lilygot547Battery = lilygo_t5_47_battery_ns.class_(
    "Lilygot547Battery", cg.PollingComponent
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Lilygot547Battery),
        cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
).extend(cv.polling_component_schema("5s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_VOLTAGE in config:
        conf = config[CONF_VOLTAGE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_voltage_sensor(sens))

    # Disable Arduino legacy ADC driver to prevent conflicts with ESP-IDF NG ADC
    cg.add_build_flag("-DARDUINO_ADC_DISABLE")
    cg.add_build_flag("-DCONFIG_DISABLE_HAL_LOCKS")
    cg.add_build_flag("-DCONFIG_ADC_DISABLE_DAC=y")
    
    # More aggressive legacy ADC driver disable flags
    cg.add_build_flag("-DCONFIG_ADC_LEGACY_DISABLE")
    cg.add_build_flag("-DCONFIG_ESP_ADC_CAL_DISABLE")
    cg.add_build_flag("-DNO_ARDUINO_ADC_INIT")
    cg.add_build_flag("-DCONFIG_DISABLE_HAL_INIT_LOCK")
