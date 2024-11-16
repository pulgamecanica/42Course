#include <iostream>
#include <thread>
#include "thread_safe_queue.hpp"

void testPush(ThreadSafeQueue<int>& p_queue, int p_value) {
    p_queue.push_back(p_value);
    std::cout << "Pushed value: " << p_value << std::endl;
}

void testPop(ThreadSafeQueue<int>& p_queue) {
    try {
        int value = p_queue.pop_front();
        std::cout << "Popped value: " << value << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

int main() {
    ThreadSafeQueue<int> myQueue;

    std::thread thread1(testPush, std::ref(myQueue), 10);
    std::thread thread2(testPush, std::ref(myQueue), 20);
    std::thread thread3(testPop, std::ref(myQueue));
    std::thread thread4(testPop, std::ref(myQueue));
    std::thread thread5(testPop, std::ref(myQueue));

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    return 0;
}

