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

LilygoT547Battery = lilygo_t5_47_battery_ns.class_(
    "LilygoT547Battery", cg.PollingComponent
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(LilygoT547Battery),
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

    # Removed epdiy library dependency for simulation-only battery component
    # The display component will provide the epdiy library
    # cg.add_library("https://github.com/vroland/epdiy.git", "v7")
    # cg.add_build_flag("-DBOARD_HAS_PSRAM")
    # cg.add_build_flag("-DCONFIG_EPD_DISPLAY_TYPE_ED047TC1")
    # cg.add_build_flag("-DCONFIG_EPD_BOARD_REVISION_LILYGO_T5_47")
    # cg.add_build_flag("-DCONFIG_EPD_DISABLE_ADC")
