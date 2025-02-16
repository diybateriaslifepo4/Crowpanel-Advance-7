#include "esphome.h"
#include "Wire.h"
#include "esphome/components/output/float_output.h"

namespace pca9557 {

class PCA9557Output : public esphome::Component, public esphome::output::FloatOutput {
 public:
  PCA9557Output(uint8_t address = 0x18) : address_(address) {}

  void setup() override {
    esphome::delay(500); // Espera 500ms para dar tiempo a la inicialización del touchpad
    // Configure all pins as outputs (register 0x03)
    uint8_t config_data[] = {0x03, 0x00};
    if (!Wire.writeBytes(address_, config_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to configure pins as outputs");
    }
    // Set initial state to all off (register 0x01)
    uint8_t output_data[] = {0x01, 0x00};
    if (!Wire.writeBytes(address_, output_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to set initial state");
    }
  }

  void write_state(float state) override {
    uint8_t current_state;
    // Read current state (register 0x00)
    Wire.beginTransmission(address_);
    Wire.write(0x00);
    Wire.endTransmission();
    if (Wire.requestFrom(address_, (uint8_t)1) != 1) {
      ESP_LOGE("PCA9557", "Failed to read current state");
      return;
    }
    current_state = Wire.read();

    // Update pin 1 (backlight) while preserving other pins
    if (state > 0.5f) {
      current_state |= (1 << 1);  // Set pin 1 high
    } else {
      current_state &= ~(1 << 1); // Set pin 1 low
    }

    // Write new state (register 0x01)
    uint8_t output_data[] = {0x01, current_state};
    if (!Wire.writeBytes(address_, output_data, 2)) {
      ESP_LOGE("PCA9557", "Failed to write new state");
    }
  }

 protected:
  uint8_t address_;
};

}  // namespace pca9557