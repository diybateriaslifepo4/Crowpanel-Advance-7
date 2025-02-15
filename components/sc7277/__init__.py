import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, spi
from esphome.const import CONF_ID

DEPENDENCIES = ["spi"]
CODEOWNERS = ["@tu_usuario_github"]

sc7277_ns = cg.esphome_ns.namespace("sc7277")
SC7277Display = sc7277_ns.class_(
    "SC7277Display", cg.PollingComponent, display.DisplayBuffer
)

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(SC7277Display),
    }
).extend(cv.polling_component_schema("100ms"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await display.register_display(var, config)