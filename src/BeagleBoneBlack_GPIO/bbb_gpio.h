#ifndef BBB_GPIO_H
#define BBB_GPIO_H

#include <unistd.h>
#include <fstream>
#include <iostream>

/* ---------- Added a few BeagleBone Black Specific Definitions ------------- */

// The BeagleBone Black has two pin headers P8 and P9. Each header has 46 pins
// This project uses the GPIO pins 7, 8, 9, 10, 15, 16, 17, 18 on the P8 header.

  constexpr const char* sysfs_gpio_path = "/sys/class/gpio";

  // We need the header numbering (PX_YY, where X is the header# and YY is the
  // header specific pin number from 0-32) to set the pinmux mode to GPIO
  constexpr const char* HeaderPinNums[] = {
    "P8_07", "P8_08", "P8_09", "P8_10", "P8_15", "P8_16", "P8_17", "P8_18"
  };
  // We also need the absolute pin numbers to set the Mode (Input/Output)
  // and State (HIGH/LOW)
  constexpr const char *AbsPinNums[]= { "66", "67", "69", "68", "47", "46", "27", "65"};

  // Pin Mode is "in" or "out"
  constexpr const char* sysfsMode[] = {"in", "out"};

/* ------------------------------------------------------------------------- */

  enum class PinState { LOW, HIGH };

  enum class PinMode { INPUT, OUTPUT };

  enum class PinID { Pin01, Pin02, Pin03, Pin04, Pin05, Pin06, Pin07, Pin08 };

  // Overload for ">>" to read INTO PinState objects
  // Eg: PinState state;  value_file >> state;
  inline std::istream& operator>>(std::istream& input_stream, PinState& state) {
    int val;
    input_stream >> val;
    state = static_cast<PinState>(val);
    return input_stream;
  }

  // Overload for "<<" to read FROM PinState objects
  // Eg: PinState state{PinState::HIGH};  value_file << state;
  inline std::ostream& operator<<(std::ostream& output_stream, PinState& state) {
    return output_stream << static_cast<int>(state);
  }

//  void sleepUs(size_t us);
//   size_t frameLengthUs();


#endif // BBB_GPIO_H
