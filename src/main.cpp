#include <atomic>
#include <csignal>
#include <stack>
#include <iostream>
#include <string>

#include "FileReader.hpp"
#include "GpioWriter.hpp"
#include "GpioFrame.hpp"

using nlohmann::json;

std::atomic<bool> running{true};
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    running = false;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [/path/to/json-config-file]" << std::endl;
        return 1;
    }

    std::string json_file = argv[1];
    std::ifstream config_fstream(json_file);
    nlohmann::json json_config = json::parse(config_fstream);

    std::string gpio_cmds_file = json_config["GPIOCmdsFile"].get<std::string>();

    FileReader reader(gpio_cmds_file);
    GpioWriter writer(json_config, reader.getBufferPoolPtr(), reader.getReadQueuePtr());

    reader.startReading();
    writer.startWriting();

    while(running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Exiting" << std::endl;
    return 0;
}
