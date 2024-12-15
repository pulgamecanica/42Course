# Welcome to libftpp

## Overview

This library provides 7 core components:

| Component | All-in-one Header | Dependencies | Description |
| --------- | ----------------- | ------------ | ----------- |
| **`Data Structures`** | `data_structures.hpp` | `pool.hpp` & `data_buffer.hpp` | This structures will help you manage and serialize data. |
| **`Design Patterns`** | `design_patterns.hpp` | `memento.hpp`, `observer.hpp`, `observable_value.hpp` `singleton.hpp` & `state_machine.hpp` | Contains a set of utils to handle different scenarios in your programs. |
| **`IOStream`** | `thread_safe_iostream.hpp`, `logger.hpp` | `thread_safe_iostream.hpp` & `logger.hpp` | Defines a structure to deal with output and input for programs using threads, and a logger. |
| **`Thread`** | `threading.hpp` | `thread_safe_queue.hpp` & `thread.hpp`, `worker_pool .hpp` & `persistent_worker.hpp` |  Which contain a set of utils to handle threads. |
| **`Network`** | `network.hpp` | `message.hpp`, `server.hpp` & `client.hpp` | A set of utils to generate a server and a client which can comunicate via the `Message` class. |
| **`Mathematics`** | `IVector2.hpp` | `IVector2.hpp`, `IVector3.hpp`, `random_2D_coordinate_generator.hpp` & `perlin_noise_2D.hpp` | Implementation of a 2D and 3D vecotor with utils and operators. Implementation of random generator and perlinnoise 2D coordinates generator. |
| **`Time`** | `Time.hpp` | `chronometer.hpp`, `timer.hpp` & `scheduler.hpp` | Time related utils. |


- *Note*: `Server` & `Client` use the `Observer` design pattern.  

- *Note*: The `PersistentWorker` uses `WorkerPool` to provide a strong and reliable thread worker, abstracting the thread logic.

## Requirements
- **C++17 or later**: The library utilizes features introduced in C++11 such as `std::unique_ptr`, `std::stack` & `std::function` (lambdas).
- **No External Dependencies**: This library does not require any external libraries.

## Installation

1. Clone the repository and build:
   ```bash
   git clone https://github.com/pulgamecanica/libftpp.git
   cd libftpp
   make all
   make test (optional)
   ```
   
2. Include the header files in your project:
   - Add include/ directory to the list of searched directories
   
3. Compile your project:
   - Ensure your build system (e.g., Makefile) includes the header files and links any required dependencies.
   - Make sure that you link the library correctly: `-L/path/to/libftpp.a -lftpp`

## Documentation

For more details on each class and method, refer to the header files.

## License

This library is licensed under the MIT License. See the LICENSE file for more details.

## Contributing

Feel free to fork the repository and submit pull requests. Contributions are welcome to improve the functionality and performance of the library.

---

### Notes:

- The library is designed to be **header-only**, so you can simply include the relevant header files in your projects.
- While this library handles basic memory pooling and object serialization needs, it can be extended with more complex features like thread-safety or more advanced serialization strategies and networking.
