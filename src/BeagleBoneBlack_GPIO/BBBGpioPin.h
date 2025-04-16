#ifndef BBB_GPIO_PIN_H
#define BBB_GPIO_PIN_H

#include "bbb_gpio.h"

class BBBGpioPin{
public:
  BBBGpioPin(const uint8_t& pin_num, const std::string& abs_pin, const std::string& hdr_pin, const std::string& mode);
  ~BBBGpioPin();

  BBBGpioPin(BBBGpioPin&& other) noexcept
          : value_in_(std::move(other.value_in_)),
            value_out_(std::move(other.value_out_)){};

  BBBGpioPin& operator=(BBBGpioPin&& other) noexcept {
      if (this != &other) {
          value_in_ = std::move(other.value_in_);
          value_out_ = std::move(other.value_out_);
      }
      return *this;
  }

  bool digitalWrite(PinState write_state);
  bool digitalRead(PinState& read_state);

private:
  uint8_t pin_num_{};
  std::string abs_pin_num_{};
  std::string pinmux_path_{};
  std::string gpio_pin_dir_{};

  std::ifstream value_in_;
  std::ofstream value_out_;

  bool configurePinmux();
  bool exportPin();
  bool setPinMode(const std::string& mode);
  bool unexportPin();
  bool pinDirExists();
};


#endif // BBB_GPIO_PIN_H
