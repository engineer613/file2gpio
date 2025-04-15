#include "FileReader.hpp"

FileReader::FileReader(std::string &file_path):
  file_path_(file_path)
{
  file_.open(file_path_, std::ios::binary);
  if (!file_.is_open()) {
    std::cout  << "[FileReader] Error Opening Command File!" << std::endl;
  } else {
    std::cout  << "[FileReader] Successfully Opened Command File!" << std::endl;
  }

  p_buffer_pool_ = std::make_shared<BufferPool<GpioBuffer, GPIO_QUEUE_SIZE>>();
  p_buffer_queue_ = std::make_shared<ThreadSafeQueue<GpioBuffer*, GPIO_QUEUE_SIZE>>();
}

FileReader::~FileReader() {
  if (reader_thread_.joinable()) {
    reader_thread_.join();
  }
  if (file_.is_open()) {
    file_.close();
  }
}

bool FileReader::startReading() {
  reader_thread_ = std::thread(&FileReader::readerThreadFunction, this);
  return true;
}

sharedBufferQueue FileReader::getReadQueuePtr() {
    return p_buffer_queue_;
}

sharedBufferPool FileReader::getBufferPoolPtr() {
    return p_buffer_pool_;
}

void FileReader::readerThreadFunction() {
  while (file_.good()) {
    // Get a new buffer, read into it, queue it up
    GpioBuffer* p_tx_buffer = p_buffer_pool_->get();
    if (file_.read(reinterpret_cast<char*>(p_tx_buffer->data()),
               sizeof(GpioBuffer))) {
      p_buffer_queue_->push(p_tx_buffer);
      //printFrames(p_tx_buffer);
      std::this_thread::sleep_for(std::chrono::microseconds(5));
    }
  }
  std::cout << "[FileReader] End of File Reached" << std::endl;
  p_buffer_queue_->setDone();
}

void FileReader::printFrames(const GpioBuffer *p_buffer) {
    for (const auto& frame: *p_buffer) {
        std::bitset<8> gpio_bits(frame.gpio_byte);
        std::cout << "GPIO: " << gpio_bits << " | Hold: " << frame.hold_s << " s" << std::endl;
    }
}
