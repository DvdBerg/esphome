#pragma once

#include "esphome/core/component.h"
#include "esphome/core/time.h"

#include "esphome/components/spi/spi.h"

namespace esphome {
namespace sn74hc595_display {

class SN74HC595DisplayComponent;

using sn74hc595_writer_t = std::function<void(SN74HC595DisplayComponent &)>;

class SN74HC595DisplayComponent : public PollingComponent,
                         public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW,
                                               spi::CLOCK_PHASE_LEADING, spi::DATA_RATE_1MHZ> {
 public:
  void set_writer(sn74hc595_writer_t &&writer);

  void setup() override;

  // void loop() override;

  void dump_config() override;

  void update() override;

  float get_setup_priority() const override;

  void display();

  // void set_intensity(uint8_t intensity);
  void set_num_boards(uint8_t num_boards);
  void set_reverse(bool reverse) { ESP_LOGI("LOG", "set_reverse: %u", reverse ); this->reverse_ = reverse; };

  /// Evaluate the printf-format and print the result at the given position.
  uint8_t printf(uint8_t pos, const char *format, ...) __attribute__((format(printf, 3, 4)));
  /// Evaluate the printf-format and print the result at position 0.
  uint8_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));

  /// Print `str` at the given position.
  uint8_t print(uint8_t pos, const char *str);
  /// Print `str` at position 0.
  uint8_t print(const char *str);

  /// Evaluate the strftime-format and print the result at the given position.
  uint8_t strftime(uint8_t pos, const char *format, ESPTime time) __attribute__((format(strftime, 3, 0)));

  /// Evaluate the strftime-format and print the result at position 0.
  uint8_t strftime(const char *format, ESPTime time) __attribute__((format(strftime, 2, 0)));

 protected:
  void send_digit_(uint8_t pos, uint8_t value);
  // void send_to_all_(uint8_t a_register, uint8_t data);

  // uint8_t intensity_{15};     // Intensity of the display from 0 to 15 (most)
  // bool intensity_changed_{};  // True if we need to re-send the intensity
  uint8_t num_boards_{1};
  uint8_t *buffer_;
  bool reverse_{false};
  optional<sn74hc595_writer_t> writer_{};
};

}  // namespace sn74hc595_display
}  // namespace esphome
