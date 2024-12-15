#include <iostream>
#include "scheduler.hpp"
#include "thread_safe_iostream.hpp"

ThreadSafeIOStream threadSafeCout;

void oneTimeTask() {
  threadSafeCout << "One-time task executed." << std::endl;
}

void repeatingTask() {
  threadSafeCout << "Repeating task executed." << std::endl;
}

int main() {
  Scheduler scheduler;

  // Schedule a one-time task
  scheduler.schedule_once(2000, oneTimeTask); // Executes after 2 seconds

  // Schedule a repeating task
  scheduler.schedule_repeating("RepeatingTask", 1000, repeatingTask); // Executes every 1 second

  // Let tasks run for 5 seconds
  std::this_thread::sleep_for(std::chrono::seconds(5));

  // Cancel the repeating task
  scheduler.cancel("RepeatingTask");
  threadSafeCout << "Repeating task canceled." << std::endl;

  return 0;
}
