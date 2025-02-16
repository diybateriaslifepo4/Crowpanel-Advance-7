#include "esphome.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/output/float_output.h"

// Make sure the ESP logging function is visible in this namespace.
using esphome::esp_log_printf_;

namespace pca9557 {

class PCA9557Output : public esphome::i2c::I2CDevice, public esphome::Component, public esphome::output::FloatOutput {
 public:
  // Use default base constructor and assign the address locally.
  PCA9557Output(uint8_t address = 0x18) : esphome::i2c::I2CDevice() { 
    this->address_ = address; 
  }

  // This method will be called by the I2C infrastructure.
  void set_i2c_master(esphome::i2c::I2CMaster *master) override {
    this->i2c_master_ = master;
  }

  void setup() override {
    esphome::delay(500); // Espera 500ms para dar tiempo a la inicialización del touchpad
    // Configure all pins as outputs (register 0x03)
    uint8_t config_data[2] = {0x03, 0x00};
    if (!write_array(config_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to configure pins as outputs");
    }
    // Set initial state to all off (register 0x01)
    uint8_t output_data[2] = {0x01, 0x00};
    if (!write_array(output_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to set initial state");
    }
  }

  void write_state(float state) override {
    uint8_t current_state;
    // Read current state (register 0x00)
    if (!read_byte(0x00, &current_state)) {
      ESP_LOGE("PCA9557", "Failed to read current state");
      return;
    }

    // Update pin 1 (backlight) while preserving other pins
    if (state > 0.5f) {
      current_state |= (1 << 1);  // Set pin 1 high
    } else {
      current_state &= ~(1 << 1); // Set pin 1 low
    }

    // Write new state (register 0x01)
    uint8_t output_data[2] = {0x01, current_state};
    if (!write_array(output_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to write new state");
    }
  }

 protected:
  uint8_t address_;
  esphome::i2c::I2CMaster *i2c_master_ = nullptr; // New member variable

  // Helper function to write an array of bytes to the device.
  bool write_array(const uint8_t *data, size_t len) {
    if (this->i2c_master_ == nullptr)
      return false;
    return this->i2c_master_->write_array(this->address_, data, len);
  }

  // Helper function to read a single byte from a register.
  bool read_byte(uint8_t reg, uint8_t *data) {
    if (this->i2c_master_ == nullptr)
      return false;
    // Write the register address with a repeated start.
    if (!this->i2c_master_->write(this->address_, &reg, 1, true))
      return false;
    return this->i2c_master_->read(this->address_, data, 1);
  }
};

}  // namespace pca9557