#include "bbb_gpio_pin.h"
#include "GpioFrame.hpp"
#include "ThreadSafeQueue.hpp"
#include "BufferPool.hpp"
#include "json.hpp"
#include <bitset>

class GpioWriter {
public:
  GpioWriter(nlohmann::json& config,
    std::shared_ptr<BufferPool<GpioBuffer, GPIO_QUEUE_SIZE>> p_buffer_pool,
    std::shared_ptr<ThreadSafeQueue<GpioBuffer*, GPIO_QUEUE_SIZE>> p_queue);

  ~GpioWriter();

  bool setHigh(); // for testing
  void startWriting();
  void stopWriting();
private:
  std::shared_ptr<BufferPool<GpioBuffer, GPIO_QUEUE_SIZE>> p_buffer_pool_;
  std::shared_ptr<ThreadSafeQueue<GpioBuffer*, GPIO_QUEUE_SIZE>> p_buffer_queue_;

  std::array<std::unique_ptr<BBBGpioPin>, 8> gpio_pins_;

  bool writing_;
  std::thread writer_thread_;
  void writerThreadFunction();
  bool writeFrame(const GpioFrame& frame);
  bool printFrame(const GpioFrame& frame);
};