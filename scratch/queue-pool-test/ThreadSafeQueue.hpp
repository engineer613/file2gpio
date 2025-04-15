/* Simple lock based thread safe FIFO queue implementation
with wraparound for a specified queue size (at compile time). Meant for use with
a single producer and single consumer threads at the moment */

#ifndef GPIO_THREADSAFEQUEUE_HPP
#define GPIO_THREADSAFEQUEUE_HPP

#include <array>
#include <mutex>
#include <condition_variable>
#include <cstddef>
#include <thread>

template <typename T, std::size_t max_size = 10>
class ThreadSafeQueue {
public:
  ThreadSafeQueue(){};

  size_t size() const { return size_; }

  bool isEmpty() const { return size_ == 0; }

  bool push(T value) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_[back_] = value;
    back_ = (back_ + 1) % max_size;
    size_++;
    new_data_available_.notify_one();
    return true;
  }

  bool pop(T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    new_data_available_.wait(lock, [this] { return size_ > 0; });
    value = std::move(queue_[front_]);
    front_ = (front_ + 1) % max_size;
    size_--;
    return true;
  }

private:
  std::mutex mutex_;
  std::condition_variable new_data_available_;
  std::array<T, max_size> queue_;
  std::size_t size_{0};
  std::size_t front_{0};
  std::size_t back_{0};
};


#endif // GPIO_THREADSAFEQUEUE_HPP
