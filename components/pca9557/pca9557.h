// Add forward declaration for I2CMaster.
namespace esphome {
  namespace i2c {
    class I2CMaster;
    // Se eliminó la redeclaración de ERROR_OK, ya que existe en i2c_bus.h.
  }
}

#include "esphome.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/output/float_output.h"

// Make sure the ESP logging function is visible in this namespace.
using esphome::esp_log_printf_;

namespace pca9557 {

class PCA9557Output : public esphome::i2c::I2CDevice, public esphome::Component, public esphome::output::FloatOutput {
 public:
  PCA9557Output(uint8_t address = 0x18) : esphome::i2c::I2CDevice() { 
    this->address_ = address; 
  }

  void setup() override {
    esphome::delay(500); // Espera 500ms para dar tiempo a la inicialización del touchpad
    uint8_t config_data[2] = {0x03, 0x00};
    if (!write_array(config_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to configure pins as outputs");
    }
    uint8_t output_data[2] = {0x01, 0x00};
    if (!write_array(output_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to set initial state");
    }
  }

  void write_state(float state) override {
    uint8_t current_state;
    if (!read_byte(0x00, &current_state)) {
      ESP_LOGE("PCA9557", "Failed to read current state");
      return;
    }
    if (state > 0.5f) {
      current_state |= (1 << 1);
    } else {
      current_state &= ~(1 << 1);
    }
    uint8_t output_data[2] = {0x01, current_state};
    if (!write_array(output_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to write new state");
    }
  }

 protected:
  uint8_t address_;
  
  // Se usan los métodos write() y read() de I2CDevice sin pasar la dirección.
  bool write_array(const uint8_t *data, size_t len) {
    return this->write(data, len, true) == esphome::i2c::ERROR_OK;
  }

  bool read_byte(uint8_t reg, uint8_t *data) {
    if (this->write(&reg, 1, true) != esphome::i2c::ERROR_OK)
      return false;
    return this->read(data, 1) == esphome::i2c::ERROR_OK;
  }
};

}  // namespace pca9557