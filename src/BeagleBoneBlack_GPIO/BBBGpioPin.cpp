#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "BBBGpioPin.h"

BBBGpioPin::BBBGpioPin(const uint8_t& pin_num,
                       const std::string &abs_pin_num,
                       const std::string &hdr_pin_num,
                       const std::string& mode)
    : pin_num_(pin_num),
      abs_pin_num_(abs_pin_num),
      pinmux_path_("/sys/devices/platform/ocp/ocp:" + hdr_pin_num + "_pinmux/state"),
      gpio_pin_dir_(std::string(sysfs_gpio_path) + "/gpio" + abs_pin_num)
{
  auto init_ok = this->unexportPin();  // Unexport first just in case pin is already exported
  init_ok &= this->configurePinmux();
  init_ok &= this->exportPin();
  init_ok &= this->setPinMode(mode);

  if (init_ok) {
    std::cout << "[GPIOPin" << abs_pin_num_ << "] has been successfully initialized\n" << std::endl;
  } else {
    std::cout << "[GPIOPin" << abs_pin_num_ << "] Error initializing Pin: " << abs_pin_num_ << " ! Please recheck configuration\n"<< std::endl;
  }
}

BBBGpioPin::~BBBGpioPin() {
  std::ofstream unexport_file("/sys/class/gpio/unexport");
  if (unexport_file) {
    std::cout << "[GPIOPin" << abs_pin_num_ << "] Unexporting Pin: " << abs_pin_num_ << std::endl;
    unexport_file << abs_pin_num_ << std::endl;
  }
}


// Helper functions
bool BBBGpioPin::pinDirExists() {
    struct stat st;
    return (stat(gpio_pin_dir_.c_str(), &st) == 0);
}

bool fileExists(const std::string& path) {
    struct stat st;
    return (stat(path.c_str(), &st) == 0);
}

// Set pinmux to GPIO
bool BBBGpioPin::configurePinmux() {
    std::ofstream pinmux_file(pinmux_path_);

    if (!pinmux_file) {
        std::cout <<"[GPIOPin" << abs_pin_num_ << "] Failed to open pinmux state for " << abs_pin_num_ << std::endl;
        return false;
    }

    pinmux_file << "gpio" << std::endl;
    pinmux_file.close();
    return true;
}

// Write abs pin num to export file
bool BBBGpioPin::exportPin() {
    std::ofstream gpio_export_file(std::string(sysfs_gpio_path) + "/export");

    if(!gpio_export_file) {
        std::cout <<"[GPIOPin" << abs_pin_num_ << "] Failed to open GPIO export!" << std::endl;
        return false;
    }
    gpio_export_file << abs_pin_num_ << std::endl;
    gpio_export_file.close();

    while(!pinDirExists()) {
        std::cout <<"[GPIOPin" << abs_pin_num_ << "] Waiting on export for Pin " << abs_pin_num_ << std::endl;
        usleep(50000);
    }
    std::cout <<"[GPIOPin" << abs_pin_num_ << "] Pin " << abs_pin_num_ << " exported successfully!" << std::endl;
    sleep(2);
    return true;
}


bool BBBGpioPin::unexportPin() {
  std::ofstream unexport_file(std::string(sysfs_gpio_path) + "/unexport");
  unexport_file << abs_pin_num_ << std::endl;
  unexport_file.close();
  return true;
}


// Set pin direction/mode ("in" or "out") in syfs direction file for pin
bool BBBGpioPin::setPinMode(const std::string& pin_mode) {
  auto mode_path = gpio_pin_dir_ + "/direction";

  while (!fileExists(mode_path)) {
    sleep(1);
    std::cout <<"[GPIOPin" << abs_pin_num_ << "] Waiting on direction file: " << mode_path << std::endl;
  }

  std::ofstream mode_file(mode_path);
  if (!mode_file) {
      perror("Cannot Open direction file");
      return false;
  }
  mode_file << pin_mode << std::endl;
  mode_file.close();
  return true;
}


bool BBBGpioPin::digitalWrite(PinState write_state) {
  value_out_.open(gpio_pin_dir_ + "/value");
  if (!value_out_) {
    perror("Couldn't open value file");
    return false;
  }

  value_out_.seekp(0);
  value_out_ << static_cast<int>(write_state) << std::endl;
  value_out_.flush();
  value_out_.close();
  return true;
}

bool BBBGpioPin::digitalRead(PinState &read_state) {
  if (value_in_) {
    value_in_.seekg(0);
    value_in_ >> read_state;
    value_in_.close();
    return true;
  }
  return false;
}
