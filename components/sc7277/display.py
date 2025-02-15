import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import CONF_ID

DEPENDENCIES = ["spi"]

sc7277_ns = cg.esphome_ns.namespace("sc7277")
SC7277Display = sc7277_ns.class_(
    "SC7277Display", cg.Component, cg.PollingComponent, display.DisplayBuffer
)

# Esta es la clave: registramos el componente para la plataforma "sc7277"
DISPLAY_PLATFORM_SCHEMA = display.FULL_DISPLAY_SCHEMA.extend({
    cv.GenerateID(CONF_ID): cv.declare_id(SC7277Display),
})

CONFIG_SCHEMA = cv.All(
    DISPLAY_PLATFORM_SCHEMA.extend({
        cv.GenerateID(): cv.declare_id(SC7277Display),
    }).extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await display.register_display(var, config)