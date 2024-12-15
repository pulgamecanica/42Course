#include "libftpp.hpp"
#include <iostream>

ThreadSafeIOStream threadSafeCout;

// Fun application that uses all available classes and structures from libftpp.hpp
int main() {
  // Logger usage
  Logger logger(std::cout);
  logger.log(LogLevel::INFO, "Logger initialized successfully.");

  // Using DataBuffer
  DataBuffer buffer;
  buffer << std::string("Test String");
  std::string output;
  buffer >> output;
  logger.log(LogLevel::INFO, "DataBuffer test: " + output);

  // Using PerlinNoise2D
  PerlinNoise2D perlin;
  float noise_value = perlin.sample(0.5, 0.8);
  logger.log(LogLevel::INFO, "Perlin Noise Sample: " + std::to_string(noise_value));

  // Using IVector2
  IVector2<int> vec2(3, 4);
  logger.log(LogLevel::INFO, "IVector2 length: " + std::to_string(vec2.length()));

  // Using IVector3
  IVector3<float> vec3(1.0f, 2.0f, 3.0f);
  logger.log(LogLevel::INFO, "IVector3 dot product: " + std::to_string(vec3.dot(vec3)));

  // Using StateMachine
  StateMachine<std::string> state_machine;
  state_machine.addState("idle");
  state_machine.addState("running");
  state_machine.addTransition("idle", "running", []() {
    threadSafeCout << "Transitioning from idle to running" << std::endl;
  });
  state_machine.transitionTo("running");

  // Using Singleton
  Singleton<int>::instantiate(42);
  int* single_instance = Singleton<int>::instance();
  logger.log(LogLevel::INFO, "Singleton instance value: " + std::to_string(*single_instance));

  // Using Scheduler
  Scheduler scheduler;
  scheduler.schedule_once(500, []() {
    threadSafeCout << "Task executed after 500ms" << std::endl;
  });

  // Ensuring Scheduler tasks run
  std::this_thread::sleep_for(std::chrono::milliseconds(600));

  // Using ThreadSafeQueue
  ThreadSafeQueue<int> ts_queue;
  ts_queue.push_back(1);
  ts_queue.push_back(2);
  int front = ts_queue.pop_front();
  logger.log(LogLevel::INFO, "ThreadSafeQueue front: " + std::to_string(front));

  // Using Thread
  Thread test_thread("TestThread", []() {
    threadSafeCout << "Thread running." << std::endl;
  });
  test_thread.start();
  test_thread.stop();

  // Using ObservableValue
  ObservableValue<int> observable(10);
  observable.subscribe([](const int& value) {
    threadSafeCout << "ObservableValue changed: " << value << std::endl;
  });
  observable.set(20);

  // Using Memento
  class TestClass : public Memento {
    friend class Memento;
   public:
    int value = 0;
   private:
    void _saveToSnapshot(Memento::Snapshot& snapshot) override {
      snapshot << value;
    }
    void _loadFromSnapshot(Memento::Snapshot& snapshot) override {
      snapshot >> value;
    }
  };
  TestClass test_object;
  test_object.value = 42;
  auto snapshot = test_object.save();
  test_object.value = 0;
  test_object.load(snapshot);
  logger.log(LogLevel::INFO, "Memento restored value: " + std::to_string(test_object.value));

  // Using PersistentWorker
  PersistentWorker worker;
  worker.addTask("task1", []() {
    threadSafeCout << "PersistentWorker task executed." << std::endl;
  });
  

  // Using Timer
  Timer timer(10);
  threadSafeCout << "Timer set for 10ms. Waiting..." << std::endl;
  while (!timer.hasTimedOut()) {
    ; // Busy wait
  }
  threadSafeCout << "Timer expired!" << std::endl;

  // Remove worker task
  worker.removeTask("task1");

  // Using Pool
  Pool<int> pool;
  pool.resize(5);
  Pool<int>::Object pooled_object = pool.acquire(32);
  logger.log(LogLevel::INFO, "Pooled object value: " + std::to_string(*pooled_object.operator->()));

  // Using Random2DCoordinateGenerator
  Random2DCoordinateGenerator random_gen(42);
  long long random_value = random_gen(5, 10);
  logger.log(LogLevel::INFO, "Random2DCoordinateGenerator value: " + std::to_string(random_value));

  // Using Server
  Server server;
  try {
    server.start(8080);
  } catch (const std::runtime_error& e) {
    logger.log(LogLevel::ERROR, "Server error: " + std::string(e.what()));
  }

  // Using Client
  Client client;
  try {
    client.connect("localhost", 8080);
    client.disconnect();
  } catch (const std::runtime_error& e) {
    logger.log(LogLevel::ERROR, "Client error: " + std::string(e.what()));
  }

  // Using ThreadSafeIOStream
  threadSafeCout.setPrefix("ThreadSafeIOStream Test: ");
  threadSafeCout << "Outputting safely." << std::endl;

  threadSafeCout << "All tests executed successfully." << std::endl;
  return 0;
}
