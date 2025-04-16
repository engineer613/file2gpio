#ifndef GPIO_GPIOFRAME_HPP
#define GPIO_GPIOFRAME_HPP

#include "BufferPool.hpp"
#include "ThreadSafeQueue.hpp"
#include <array>
#include <cstddef>

// TODO: Use flat binary buffers instead

// Pack the GPIOFrame struct so there are no padded bytes
#pragma pack(push, 1)
struct GpioFrame {
  uint8_t gpio_byte{0}; // 8 bit GPIO pin values
  int32_t hold_s{0}; // Hold time (seconds) since we're emulating recorded video game commands

  void reset() {
    gpio_byte = 0;
    hold_s = 0;
  }
};
#pragma pack(pop)


constexpr size_t GPIO_BUFFER_SIZE = 50; // Number of GPIOFrames in a buffer
using GpioBuffer = std::array<GpioFrame, GPIO_BUFFER_SIZE>;

constexpr size_t GPIO_QUEUE_SIZE = 50; // Number of GPIOBuffers in the queue
using sharedBufferQueue = std::shared_ptr<ThreadSafeQueue<GpioBuffer*, GPIO_QUEUE_SIZE>>;
using sharedBufferPool = std::shared_ptr<BufferPool<GpioBuffer, GPIO_QUEUE_SIZE>>;

#endif // GPIO_GPIOFRAME_HPP
