#include <iostream>
#include <memory>
#include <string>

#include "BufferPool.hpp"
#include "ThreadSafeQueue.hpp"

BufferPool<int, 50> pool;
ThreadSafeQueue<int*, 50> queue;

int num_ints = 100;

void producer() {
    int * buffer;
    for (int i = 0; i <=num_ints; i++) {
        while (!(buffer = pool.get())) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        *buffer = i;
        auto push_ok = queue.push(buffer);
    }
}

void consumer() {
    int* p_buffer;
    for (int i = 0; i <= num_ints; i++) {
        auto pop_ok = queue.pop(p_buffer);
        std::cout << "Popped: " << *p_buffer << std::endl;
        pool.release(p_buffer);
    }
}


int main()
{
    std::thread p_thread(producer);
    std::thread c_thread(consumer);

    p_thread.join();
    c_thread.join();

    return 0;
}