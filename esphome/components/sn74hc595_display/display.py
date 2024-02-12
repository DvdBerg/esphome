import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, spi
from esphome.const import CONF_ID, CONF_LAMBDA

DEPENDENCIES = ["spi"]

sn74hc595_display_ns = cg.esphome_ns.namespace("sn74hc595_display")
SN74HC595DisplayComponent = sn74hc595_display_ns.class_(
    "SN74HC595DisplayComponent", cg.PollingComponent, spi.SPIDevice
)
SN74HC595DisplayComponentRef = SN74HC595DisplayComponent.operator("ref")

CONF_NUM_BOARDS = "num_boards"
CONF_REVERSE_ENABLE = "reverse_enable"

CONFIG_SCHEMA = (
    display.BASIC_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(SN74HC595DisplayComponent),
            cv.Optional(CONF_NUM_BOARDS, default=1): cv.int_range(min=1, max=255),
            cv.Optional(CONF_REVERSE_ENABLE, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("1s"))
    .extend(spi.spi_device_schema())
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await spi.register_spi_device(var, config)
    await display.register_display(var, config)

    cg.add(var.set_num_boards(config[CONF_NUM_BOARDS]))
    cg.add(var.set_reverse(config[CONF_REVERSE_ENABLE]))

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(SN74HC595DisplayComponentRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
