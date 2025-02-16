#include "esphome.h"
#include "esphome/components/i2c/i2c.h"
// Removed: #include "esphome/core/hal/i2c.h"  // No such file, base header already provides I2CMaster.
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
  // i2c_master_ is inherited from I2CDevice.
  bool write_array(const uint8_t *data, size_t len) {
    if (this->i2c_master_ == nullptr)
      return false;
    return this->i2c_master_->write_array(this->address_, data, len);
  }

  bool read_byte(uint8_t reg, uint8_t *data) {
    if (this->i2c_master_ == nullptr)
      return false;
    if (!this->i2c_master_->write(this->address_, &reg, 1, true))
      return false;
    return this->i2c_master_->read(this->address_, data, 1);
  }
};

}  // namespace pca9557