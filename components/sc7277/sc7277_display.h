#pragma once

#include "esphome/core/component.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome {
namespace sc7277 {

class SC7277Display : public display::DisplayBuffer {
 public:
  SC7277Display() : DisplayBuffer(800, 480) {}

  void setup() override;
  void update() override;
  void display() override;

 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;

 private:
  spi_device_handle_t spi_;
  void send_command(uint8_t cmd, const uint8_t *data, size_t len);
};

}  // namespace sc7277
}  // namespace esphome