#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace pca9557 {

class PCA9557Output : public Component, public output::FloatOutput, public i2c::I2CDevice {
 public:
  void setup() override {
    // Configure all pins as outputs (register 0x03)
    uint8_t config_data[] = {0x03, 0x00}; // 0x03 is the configuration register, 0x00 sets all pins as outputs
    if (!this->write_bytes(config_data[0], &config_data[1], 1)) {
      ESP_LOGE("PCA9557", "Failed to configure pins as outputs");
      this->mark_failed(); // Mark the component as failed if configuration fails
      return;
    }

    // Set initial state to all off (register 0x01)
    uint8_t output_data[] = {0x01, 0x00}; // 0x01 is the output register, 0x00 sets all outputs to LOW
    if (!this->write_bytes(output_data[0], &output_data[1], 1)) {
      ESP_LOGE("PCA9557", "Failed to set initial state");
      this->mark_failed(); // Mark the component as failed if setting initial state fails
      return;
    }

    ESP_LOGI("PCA9557", "Initialization complete");
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
    if (!this->write_byte(0x01, current_state)) {
      ESP_LOGE("PCA9557", "Failed to write new state");
      return;
    }

    ESP_LOGD("PCA9557", "Updated state to 0x%02X", current_state);
  }
};

}  // namespace pca9557
}  // namespace esphome