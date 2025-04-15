#include <atomic>
#include <csignal>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include "../src/Utils/json.hpp"

using nlohmann::json;

std::atomic<bool> running{true};
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    running = false;
}


int main() {
    std::string json_file = "./config.json";
    std::ifstream jfs(json_file);
    nlohmann::json json_config = json::parse(jfs);

    for (const auto& [key,value]: json_config["GPIOPins"].items()) {
        std::cout << key << std::endl;
        std::cout << value["HdrNum"] << std::endl;
        std::cout << "\n\n" << std::endl;
    }
    
    return 0;
}
