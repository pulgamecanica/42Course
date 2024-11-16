#include <iostream>
#include <chrono>
#include "thread.hpp"
#include "thread_safe_iostream.hpp"

void myFunction1() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Hello from Function1, iteration " << i << std::endl;
    }
}

void myFunction2() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Hello from Function2, iteration " << i << std::endl;
    }
}

int main() {
    Thread thread1("Thread1", myFunction1);
    Thread thread2("Thread2", myFunction2);

    thread1.start();
    thread2.start();

    thread1.stop();
    thread2.stop();

    return 0;
}
