#include "sc7277_display.h"
#include "esphome/core/log.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

namespace esphome {
namespace sc7277 {

static const char *TAG = "sc7277";

#define PIN_SPI_CS 5
#define PIN_SPI_CLK 6
#define PIN_SPI_MOSI 7
#define PIN_SPI_MISO 8

void SC7277Display::setup() {
  ESP_LOGI(TAG, "Setting up SC7277 display...");

  // Configuración inicial de pines para el bus RGB
  pinMode(21, OUTPUT);  // D0
  pinMode(47, OUTPUT);  // D1
  pinMode(48, OUTPUT);  // D2
  pinMode(45, OUTPUT);  // D3
  pinMode(38, OUTPUT);  // D4
  pinMode(9, OUTPUT);   // G0
  pinMode(10, OUTPUT);  // G1
  pinMode(11, OUTPUT);  // G2
  pinMode(12, OUTPUT);  // G3
  pinMode(13, OUTPUT);  // G4
  pinMode(14, OUTPUT);  // G5
  pinMode(7, OUTPUT);   // R0
  pinMode(17, OUTPUT);  // R1
  pinMode(18, OUTPUT);  // R2
  pinMode(3, OUTPUT);   // R3
  pinMode(46, OUTPUT);  // R4

  pinMode(42, OUTPUT); // HEN
  pinMode(41, OUTPUT); // VSYNC
  pinMode(40, OUTPUT); // HSYNC
  pinMode(39, OUTPUT); // PCLK

  // Configuración de la interfaz SPI
  spi_bus_config_t buscfg = {};
  buscfg.mosi_io_num = PIN_SPI_MOSI;
  buscfg.miso_io_num = PIN_SPI_MISO;
  buscfg.sclk_io_num = PIN_SPI_CLK;
  buscfg.quadwp_io_num = -1;
  buscfg.quadhd_io_num = -1;
  spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);

  spi_device_interface_config_t devcfg = {};
  devcfg.clock_speed_hz = 1000000;
  devcfg.mode = 0;
  devcfg.spics_io_num = PIN_SPI_CS;
  devcfg.queue_size = 7;
  spi_bus_add_device(HSPI_HOST, &devcfg, &spi_);

  ESP_LOGI(TAG, "SC7277 display initialized successfully");
}

void SC7277Display::update() {
  this->do_update_();
}

void SC7277Display::display() {
  ESP_LOGI(TAG, "Updating SC7277 display...");
}

void SC7277Display::draw_absolute_pixel_internal(int x, int y, Color color) {
  // Lógica para manejar cada píxel si es necesario
}

void SC7277Display::send_command(uint8_t cmd, const uint8_t *data, size_t len) {
  spi_transaction_t t = {};
  t.length = 8;
  t.tx_buffer = &cmd;
  spi_device_transmit(spi_, &t);
  if (data != nullptr && len > 0) {
    t.length = len * 8;
    t.tx_buffer = data;
    spi_device_transmit(spi_, &t);
  }
}

}  // namespace sc7277
}  // namespace esphome