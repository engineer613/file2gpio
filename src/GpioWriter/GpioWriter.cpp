#include "GpioWriter.hpp"

using json = nlohmann::json;

GpioWriter::GpioWriter(nlohmann::json& json_config,
                       sharedBufferPool p_buffer_pool,
                       sharedBufferQueue p_buffer_queue)
    :
      p_buffer_pool_(std::move(p_buffer_pool)),
      p_buffer_queue_(std::move(p_buffer_queue))
{
    for (auto it = json_config["GPIOPins"].begin(); it != json_config["GPIOPins"].end(); ++it) {
        const std::string& key = it.key();
        const auto& value = it.value();

        gpio_pins_.at(value["Num"].get<int>() - 1) = std::make_unique<BBBGpioPin>(
                value["Num"].get<int>(),
                value["AbsNum"].get<std::string>(),
                value["HdrNum"].get<std::string>(),
                value["Mode"].get<std::string>());
    }
}

GpioWriter::~GpioWriter() {
    if(writer_thread_.joinable()) {
        writer_thread_.join();
    }
}


void GpioWriter::startWriting() {
    writing_ = true;
    writer_thread_ = std::thread(&GpioWriter::writerThreadFunction, this);
}

void GpioWriter::stopWriting() {
    writing_ = false;
}

bool GpioWriter::writeFrame(const GpioFrame& frame) {
    bool write_ok = true;
    for (int i=0; i < gpio_pins_.size(); i++) {
        write_ok &= gpio_pins_[i]->digitalWrite( static_cast<PinState>((frame.gpio_byte >> i) & 1) );
    }
    printFrame(frame);
    std::this_thread::sleep_for(std::chrono::seconds(frame.hold_s));
    return write_ok;
}

// For testing/debugging
bool GpioWriter::setHigh() {
    bool write_ok = true;
    for (int i = 0; i < gpio_pins_.size(); i++) {
        write_ok &= gpio_pins_[i]->digitalWrite(PinState::HIGH);
    }

    std::this_thread::sleep_for(std::chrono::minutes(5));
    return write_ok;
}

void GpioWriter::writerThreadFunction() {
    GpioBuffer* read_buffer;
    while(writing_) {
        p_buffer_queue_->pop(read_buffer);
        if (read_buffer) {
           for (const auto& read_frame: *read_buffer) {
                writeFrame(read_frame);
                //printFrame(read_frame);
            }
            // Release the buffer here back to the buffer pool
            p_buffer_pool_->release(read_buffer);
        } else if (p_buffer_queue_->isDone()){
            std::cout << "[GPIOWriter] FileReader is done and Buffer Queue Is Empty. Exiting.." << std::endl;
            break;
        } else {
            std::cout << "[GPIOWriter] Read Queue is empty. Waiting for more data.." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

bool GpioWriter::printFrame(const GpioFrame& frame) {
    std::bitset<8> gpio_bits(frame.gpio_byte);
    std::cout << "[GpioWriter] Wrote: " << gpio_bits << " | Holding for: " << frame.hold_s << " s" << std::endl;
}