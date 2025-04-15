/* Reads GPIO commands and hold durations from file into buffers and queues them up */
#ifndef GPIO_FILEREADER_HPP
#define GPIO_FILEREADER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <thread>
#include <bitset>

#include "ThreadSafeQueue.hpp"
#include "BufferPool.hpp"
#include "GpioFrame.hpp"


class FileReader {
  public:
    FileReader(std::string& file_path);
    ~FileReader();
    bool startReading();
    sharedBufferQueue getReadQueuePtr();
    sharedBufferPool getBufferPoolPtr();

  private:
    std::string file_path_;
    std::ifstream file_;

    std::thread reader_thread_;
    void readerThreadFunction();
    void printFrames(const GpioBuffer* p_buffer);
    GpioBuffer* tx_buffer_{nullptr};

    sharedBufferPool p_buffer_pool_;
    sharedBufferQueue p_buffer_queue_;
};

#endif // GPIO_FILEREADER_HPP
