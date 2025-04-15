/** NOT a unit test. Testing control of a BBB GPIO pin with the BBBGpioPin Class **/
#include "../src/bbb_gpio/bbb_gpio_pin.h"
#include <atomic>
#include <csignal>
#include <iostream>
#include <string>


std::atomic<bool> running{true};
void signalHandler(int signum) {
  std::cout << "Interrupt signal (" << signum << ") received.\n";
  running = false;
}


int main() {
  std::string pin_name = "Pin01";
  std::string abs_pin_num = AbsPinNums[0];    // 66
  std::string hdr_pin_num = HeaderPinNums[0]; // P8_07

  BBBGpioPin pin1(pin_name, abs_pin_num, hdr_pin_num, PinMode::OUTPUT);
  signal(SIGINT, signalHandler);

  while(running) {

    sleep(1);

    std::cout << "Turning Pin High.." << std::endl;
    auto write_ok = pin1.digitalWrite(PinState::HIGH);
    sleep(10);

    std::cout << "Turning Pin Low.." << std::endl;
    write_ok = pin1.digitalWrite(PinState::LOW);
    sleep(5);
  }
  std::cout << "Exiting" << std::endl;
  return 0;
}
