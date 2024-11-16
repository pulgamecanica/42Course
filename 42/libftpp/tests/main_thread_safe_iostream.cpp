#include <iostream>
#include <thread>
#include "thread_safe_iostream.hpp"

ThreadSafeIOStream threadSafeCout;

void printNumbers(const std::string& p_prefix) {
    threadSafeCout.setPrefix(p_prefix);
    for (int i = 1; i <= 5; ++i) {
        threadSafeCout << "Number: " << i << std::endl;
    }
}

int main() {
    std::string prefix1 = "[Thread 1] ";
    std::string prefix2 = "[Thread 2] ";

    std::thread thread1(printNumbers, prefix1);
    std::thread thread2(printNumbers, prefix2);

    thread1.join();
    thread2.join();

    return 0;
}
