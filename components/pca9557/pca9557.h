#include "esphome.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/i2c/i2c.h"

namespace pca9557 {

class PCA9557Output : public esphome::i2c::I2CDevice, public esphome::output::FloatOutput {
 public:
  PCA9557Output(uint8_t address = 0x18) : esphome::i2c::I2CDevice(address) {}

  void setup() override {
    esphome::delay(500); // Espera 500ms para dar tiempo a la inicialización del touchpad
    // Configure all pins as outputs (register 0x03)
    uint8_t config_data[2] = {0x03, 0x00};
    if (!this->write_array(config_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to configure pins as outputs");
    }
    // Set initial state to all off (register 0x01)
    uint8_t output_data[2] = {0x01, 0x00};
    if (!this->write_array(output_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to set initial state");
    }
  }

  void write_state(float state) override {
    uint8_t current_state;
    // Read current state (register 0x00)
    if (!this->read_byte(0x00, &current_state)) {
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
    if (!this->write_array(output_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to write new state");
    }
  }

 protected:
  uint8_t address_;
};

}  // namespace pca9557