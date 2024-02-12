#include "sn74hc595_display.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace sn74hc595_display {

static const char *const TAG = "sn74hc595_display";

// static const uint8_t MAX7219_REGISTER_NOOP = 0x00;
// static const uint8_t MAX7219_REGISTER_DECODE_MODE = 0x09;
// static const uint8_t MAX7219_REGISTER_INTENSITY = 0x0A;
// static const uint8_t MAX7219_REGISTER_SCAN_LIMIT = 0x0B;
// static const uint8_t MAX7219_REGISTER_SHUTDOWN = 0x0C;
// static const uint8_t MAX7219_REGISTER_TEST = 0x0F;
static const uint8_t SN74HC595_UNKNOWN_CHAR = 0b00000000;

const uint8_t SN74HC595_ASCII_TO_RAW[95] PROGMEM = {
    0b11111111,              // ' ', ord 0x20
    0b10110000,              // '!', ord 0x21
    0b00100010,              // '"', ord 0x22
    SN74HC595_UNKNOWN_CHAR,  // '#', ord 0x23
    SN74HC595_UNKNOWN_CHAR,  // '$', ord 0x24
    0b01001001,              // '%', ord 0x25
    SN74HC595_UNKNOWN_CHAR,  // '&', ord 0x26
    0b00000010,              // ''', ord 0x27
    0b01001110,              // '(', ord 0x28
    0b01111000,              // ')', ord 0x29
    0b01000000,              // '*', ord 0x2A
    SN74HC595_UNKNOWN_CHAR,  // '+', ord 0x2B
    0b00010000,              // ',', ord 0x2C
    0b00000001,              // '-', ord 0x2D
    0b10000000,              // '.', ord 0x2E
    SN74HC595_UNKNOWN_CHAR,  // '/', ord 0x2F
    0b11000000,              // '0', ord 0xC0
    0b11111001,              // '1', ord 0xF9
    0b10100100,              // '2', ord 0xA4
    0b10110000,              // '3', ord 0xB0
    0b10011001,              // '4', ord 0x99
    0b10010010,              // '5', ord 0x92
    0b10000010,              // '6', ord 0x82
    0b11111000,              // '7', ord 0xF8
    0b10000000,              // '8', ord 0x80
    0b10010000,              // '9', ord 0x90
    0b01001000,              // ':', ord 0x3A
    0b01011000,              // ';', ord 0x3B
    SN74HC595_UNKNOWN_CHAR,  // '<', ord 0x3C
    0b00001001,              // '=', ord 0x3D
    SN74HC595_UNKNOWN_CHAR,  // '>', ord 0x3E
    0b01100101,              // '?', ord 0x3F
    0b01101111,              // '@', ord 0x40
    0b01110111,              // 'A', ord 0x41
    0b00011111,              // 'B', ord 0x42
    0b01001110,              // 'C', ord 0x43
    0b00111101,              // 'D', ord 0x44
    0b01001111,              // 'E', ord 0x45
    0b01000111,              // 'F', ord 0x46
    0b01011110,              // 'G', ord 0x47
    0b00110111,              // 'H', ord 0x48
    0b00110000,              // 'I', ord 0x49
    0b00111100,              // 'J', ord 0x4A
    SN74HC595_UNKNOWN_CHAR,  // 'K', ord 0x4B
    0b00001110,              // 'L', ord 0x4C
    SN74HC595_UNKNOWN_CHAR,  // 'M', ord 0x4D
    0b00010101,              // 'N', ord 0x4E
    0b01111110,              // 'O', ord 0x4F
    0b01100111,              // 'P', ord 0x50
    0b11111110,              // 'Q', ord 0x51
    0b00000101,              // 'R', ord 0x52
    0b01011011,              // 'S', ord 0x53
    0b00000111,              // 'T', ord 0x54
    0b00111110,              // 'U', ord 0x55
    0b00111110,              // 'V', ord 0x56
    0b00111111,              // 'W', ord 0x57
    SN74HC595_UNKNOWN_CHAR,  // 'X', ord 0x58
    0b00100111,              // 'Y', ord 0x59
    0b01101101,              // 'Z', ord 0x5A
    0b01001110,              // '[', ord 0x5B
    SN74HC595_UNKNOWN_CHAR,  // '\', ord 0x5C
    0b01111000,              // ']', ord 0x5D
    SN74HC595_UNKNOWN_CHAR,  // '^', ord 0x5E
    0b00001000,              // '_', ord 0x5F
    0b00100000,              // '`', ord 0x60
    0b01110111,              // 'a', ord 0x61
    0b00011111,              // 'b', ord 0x62
    0b00001101,              // 'c', ord 0x63
    0b00111101,              // 'd', ord 0x64
    0b01001111,              // 'e', ord 0x65
    0b01000111,              // 'f', ord 0x66
    0b01011110,              // 'g', ord 0x67
    0b00010111,              // 'h', ord 0x68
    0b00010000,              // 'i', ord 0x69
    0b00111100,              // 'j', ord 0x6A
    SN74HC595_UNKNOWN_CHAR,  // 'k', ord 0x6B
    0b00001110,              // 'l', ord 0x6C
    SN74HC595_UNKNOWN_CHAR,  // 'm', ord 0x6D
    0b00010101,              // 'n', ord 0x6E
    0b00011101,              // 'o', ord 0x6F
    0b01100111,              // 'p', ord 0x70
    SN74HC595_UNKNOWN_CHAR,  // 'q', ord 0x71
    0b00000101,              // 'r', ord 0x72
    0b01011011,              // 's', ord 0x73
    0b00000111,              // 't', ord 0x74
    0b00011100,              // 'u', ord 0x75
    0b00011100,              // 'v', ord 0x76
    SN74HC595_UNKNOWN_CHAR,  // 'w', ord 0x77
    SN74HC595_UNKNOWN_CHAR,  // 'x', ord 0x78
    0b00100111,              // 'y', ord 0x79
    SN74HC595_UNKNOWN_CHAR,  // 'z', ord 0x7A
    0b00110001,              // '{', ord 0x7B
    0b00000110,              // '|', ord 0x7C
    0b00000111,              // '}', ord 0x7D
    0b01100011,              // '~', ord 0x7E (degree symbol)
};

float SN74HC595DisplayComponent::get_setup_priority() const { return setup_priority::PROCESSOR; }
void SN74HC595DisplayComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SN74HC595...");
  this->spi_setup();
  this->buffer_ = new uint8_t[this->num_boards_ * 8];  // NOLINT
  for (uint8_t i = 0; i < this->num_boards_ * 8; i++)
    this->buffer_[i] = 0xFF;

  // // let's assume the user has all 8 digits connected, only important in daisy chained setups anyway
  // this->send_to_all_(MAX7219_REGISTER_SCAN_LIMIT, 7);
  // // let's use our own ASCII -> led pattern encoding
  // this->send_to_all_(MAX7219_REGISTER_DECODE_MODE, 0);
  // this->send_to_all_(MAX7219_REGISTER_INTENSITY, this->intensity_);
  this->display();
  // // power up
  // this->send_to_all_(MAX7219_REGISTER_TEST, 0);
  // this->send_to_all_(MAX7219_REGISTER_SHUTDOWN, 1);
}

// void SN74HC595DisplayComponent::loop() {
//   this->display();
// }

void SN74HC595DisplayComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "SN74HC595:");
  ESP_LOGCONFIG(TAG, "  Number of Boards: %u", this->num_boards_);
  // ESP_LOGCONFIG(TAG, "  Intensity: %u", this->intensity_);
  LOG_PIN("  CS Pin: ", this->cs_);
  LOG_UPDATE_INTERVAL(this);
}

void SN74HC595DisplayComponent::display() {
  for (uint8_t i = 0; i < 8; i++) {
    // this->enable();
    for (uint8_t j = 0; j < this->num_boards_; j++) {
      if (reverse_) {
        this->send_digit_(7 - i, buffer_[(num_boards_ - j - 1) * 8 + i]);
      } else {
        this->send_digit_(7 - i, buffer_[j * 8 + i]);
      }
    }
    this->enable();
    this->disable();
  }
}
void SN74HC595DisplayComponent::send_digit_(uint8_t pos, uint8_t value) {
  uint8_t pos_value = ( ((0x80>>pos) << 4) | ((0x80>>pos) >> 4) );
  // uint8_t pos_value = 1 << (pos);
  this->write_byte(value);
  this->write_byte(pos_value);
  // ESP_LOGI(TAG, "send_digit_: pos %x, value %x, pos shift %x", pos, value, pos_value);
}
// void SN74HC595DisplayComponent::send_to_all_(uint8_t a_register, uint8_t data) {
//   this->enable();
//   for (uint8_t i = 0; i < this->num_boards_; i++)
//     this->send_byte_(a_register, data);
//   this->disable();
// }
void SN74HC595DisplayComponent::update() {
  // if (this->intensity_changed_) {
  //   this->send_to_all_(MAX7219_REGISTER_INTENSITY, this->intensity_);
  //   this->intensity_changed_ = false;
  // }
  for (uint8_t i = 0; i < this->num_boards_ * 8; i++)
    this->buffer_[i] = 0xFF;
  if (this->writer_.has_value())
    (*this->writer_)(*this);
  this->display();
}
uint8_t SN74HC595DisplayComponent::print(uint8_t start_pos, const char *str) {
  uint8_t pos = start_pos;
  for (; *str != '\0'; str++) {
    uint8_t data = SN74HC595_UNKNOWN_CHAR;
    if (*str >= ' ' && *str <= '~')
      data = progmem_read_byte(&SN74HC595_ASCII_TO_RAW[*str - ' ']);

    if (data == SN74HC595_UNKNOWN_CHAR) {
      ESP_LOGW(TAG, "Encountered character '%c' with no SN74HC595 representation while translating string!", *str);
    }
    if (*str == '.') {
      if (pos != start_pos)
        pos--;
      this->buffer_[pos] &= 0b01111111;
    } else {
      if (pos >= this->num_boards_ * 8) {
        ESP_LOGE(TAG, "SN74HC595 String is too long for the display!");
        break;
      }
      this->buffer_[pos] = data;
    }
    pos++;
  }
  // ESP_LOGI(TAG, "printf: [%x,%x,%x,%x,%x,%x,%x,%x]", this->buffer_[0],this->buffer_[1],this->buffer_[2],this->buffer_[3],this->buffer_[4],this->buffer_[5],this->buffer_[6],this->buffer_[7]);
  return pos - start_pos;
}
uint8_t SN74HC595DisplayComponent::print(const char *str) { return this->print(0, str); }
uint8_t SN74HC595DisplayComponent::printf(uint8_t pos, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    return this->print(pos, buffer);
  return 0;
}
uint8_t SN74HC595DisplayComponent::printf(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    return this->print(buffer);
  return 0;
}
void SN74HC595DisplayComponent::set_writer(sn74hc595_writer_t &&writer) { this->writer_ = writer; }
// void SN74HC595DisplayComponent::set_intensity(uint8_t intensity) {
//   intensity &= 0xF;
//   if (intensity != this->intensity_) {
//     this->intensity_changed_ = true;
//     this->intensity_ = intensity;
//   }
// }
void SN74HC595DisplayComponent::set_num_boards(uint8_t num_boards) { this->num_boards_ = num_boards; }

uint8_t SN74HC595DisplayComponent::strftime(uint8_t pos, const char *format, ESPTime time) {
  char buffer[64];
  size_t ret = time.strftime(buffer, sizeof(buffer), format);
  if (ret > 0)
    return this->print(pos, buffer);
  return 0;
}
uint8_t SN74HC595DisplayComponent::strftime(const char *format, ESPTime time) {
  return this->strftime(0, format, time);
}

}  // namespace sn74hc595_display
}  // namespace esphome
