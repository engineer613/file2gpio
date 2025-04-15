/** NOT a unit test. Testing GPIO control on the BBB **/
#include <iostream>
#include <fstream>
#include <unistd.h>

std::string value_path = "/sys/class/gpio/gpio66/value";

void fileWrite(std::string val){
  std::ofstream value_file(value_path, std::ofstream::out);
  if (value_file) {
    value_file << val << std::endl;
  } else {
    std::cout << "Write Error" << std::endl;
  }
  value_file.close();
}

int main() {
  std::string pinmux_path = "/sys/devices/platform/ocp/ocp:P8_07_pinmux/state";
  std::string pin_dir = "/sys/class/gpio/gpio66";
  std::string export_path = "/sys/class/gpio/export";

  std::ofstream pinmux(pinmux_path, std::ofstream::out);
  if (pinmux) {
    pinmux << "gpio" << std::endl;
  }
  pinmux.close();

  std::ofstream export_file(export_path, std::ofstream::out);
  if (export_file) {
    export_file << "66" << std::endl;
  }
  export_file.close();

  sleep(5);

  std::ofstream direction_file(pin_dir + "/direction", std::ofstream::out);
  if (direction_file) {
    direction_file << "out" << std::endl;
  }
  direction_file.close();


  while (true) {
    std::cout << "HIGH" << std::endl;
    fileWrite("1");
    sleep(5);

    std::cout <<"LOW" << std::endl;
    fileWrite("0");
    sleep(5);
  }

  return 0;
}
