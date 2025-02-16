import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output, i2c
from esphome.const import CONF_ID, CONF_ADDRESS

DEPENDENCIES = ['i2c']

pca9557_ns = cg.esphome_ns.namespace('pca9557')
PCA9557Output = pca9557_ns.class_('PCA9557Output', output.FloatOutput, cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(PCA9557Output),
    cv.Required(CONF_ADDRESS): cv.i2c_address,
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x18))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await output.register_output(var, config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)