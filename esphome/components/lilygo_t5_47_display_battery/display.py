import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, sensor
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA,
    CONF_PAGES,
    CONF_VOLTAGE,
    UNIT_VOLT,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ["esp32"]

CONF_CLEAR = "clear"
CONF_TEMPERATURE = "temperature"
CONF_LANDSCAPE = "landscape"
CONF_POWER_OFF_DELAY_ENABLED = "power_off_delay_enabled"
CONF_BATTERY = "battery"

lilygo_t5_47_display_battery_ns = cg.esphome_ns.namespace("lilygo_t5_47_display_battery")

LilygoT547DisplayBattery = lilygo_t5_47_display_battery_ns.class_(
    "LilygoT547DisplayBattery", display.DisplayBuffer
)

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(LilygoT547DisplayBattery),
            cv.Optional(CONF_CLEAR, default=True): cv.boolean,
            cv.Optional(CONF_POWER_OFF_DELAY_ENABLED, default=False): cv.boolean,
            cv.Optional(CONF_LANDSCAPE, default=True): cv.boolean,
            cv.Optional(CONF_TEMPERATURE, default=23): cv.uint32_t,
            cv.Optional(CONF_BATTERY): cv.Schema({
                cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
                    unit_of_measurement=UNIT_VOLT,
                    accuracy_decimals=2,
                    device_class=DEVICE_CLASS_VOLTAGE,
                    state_class=STATE_CLASS_MEASUREMENT,
                ),
            }),
        }
    ).extend(cv.polling_component_schema("5s")),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await display.register_display(var, config)

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(display.DisplayBufferRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))

    cg.add(var.set_clear_screen(config[CONF_CLEAR]))
    cg.add(var.set_temperature(config[CONF_TEMPERATURE]))
    cg.add(var.set_landscape(config[CONF_LANDSCAPE]))
    cg.add(var.set_power_off_delay_enabled(config[CONF_POWER_OFF_DELAY_ENABLED]))

    # Handle battery configuration
    if CONF_BATTERY in config:
        battery_config = config[CONF_BATTERY]
        if CONF_VOLTAGE in battery_config:
            voltage_config = battery_config[CONF_VOLTAGE]
            voltage_sensor = await sensor.new_sensor(voltage_config)
            cg.add(var.set_voltage_sensor(voltage_sensor))

    # Add epdiy library and build flags
    cg.add_library("https://github.com/vroland/epdiy.git")
    cg.add_build_flag("-DBOARD_HAS_PSRAM")
    cg.add_build_flag("-DCONFIG_EPD_DISPLAY_TYPE_ED047TC1")
    cg.add_build_flag("-DCONFIG_EPD_BOARD_REVISION_LILYGO_T5_47")
    # Disable ADC in epdiy to prevent conflicts
    cg.add_build_flag("-DCONFIG_EPD_DISABLE_ADC")
