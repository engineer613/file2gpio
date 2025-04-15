#include <atomic>
#include <csignal>
#include <stack>
#include <iostream>
#include <string>

#include "GpioWriter.hpp"
#include "GpioFrame.hpp"

using nlohmann::json;

std::atomic<bool> running{true};
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    running = false;
}


int main() {
    std::string json_file = "./config.json";
    std::ifstream jfs(json_file);
    nlohmann::json pin_config = json::parse(jfs);

    GpioWriter writer(pin_config, nullptr, nullptr);

    writer.setHigh();


    //    GpioWriter()

    //    while(running) {
    //
    //        sleep(1);
    //
    //        std::cout << "Turning Pin High.." << std::endl;
    //        auto write_ok = pin1.digitalWrite(PinState::HIGH);
    //        sleep(10);
    //
    //        std::cout << "Turning Pin Low.." << std::endl;
    //        write_ok = pin1.digitalWrite(PinState::LOW);
    //        sleep(5);
    //    }
    std::cout << "Exiting" << std::endl;
    return 0;
}
