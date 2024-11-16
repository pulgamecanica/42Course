# Welcome to the Object Pool and Data Buffer Library

## Overview

This library provides two core components:

- **`Pool`**: A memory management class that implements a resource pool for reusable objects, ensuring efficient allocation and deallocation of resources.
- **`DataBuffer`**: A polymorphic container that handles the serialization and deserialization of objects into a byte format, supporting custom types and built-in types.

Both components are designed to help developers manage memory efficiently and serialize objects seamlessly, making them ideal for scenarios where object reuse and data storage are essential.

## Requirements

- **C++17 or later**: The library utilizes features introduced in C++11 such as `std::unique_ptr`, `std::stack`, and `std::is_trivially_copyable`.
- **No External Dependencies**: This library does not require any external libraries.

## Installation

1. Clone the repository and build:
   ```bash
   git clone https://github.com/pulgamecanica/libftpp.git
   cd libftpp
   make all
   ```
   
2. Include the header files in your project:
   - Add include/ directory to the list of searched directories
   
3. Compile your project:
   - Ensure your build system (e.g., CMake, Makefile) includes the header files and links any required dependencies.
   - Make sure that you link the library correctly: `-L/path/to/libftpp.a -lftpp`

## Documentation

For more details on each class and method, refer to the header files:

- <a href="./data_structures.html">Pool Documentation</a>
- <a href="./data_structures.html">DataBuffer Documentation</a>

## License

This library is licensed under the MIT License. See the LICENSE file for more details.

## Contributing

Feel free to fork the repository and submit pull requests. Contributions are welcome to improve the functionality and performance of the library.

---

### Notes:

- The library is designed to be **header-only**, so you can simply include the relevant header files in your projects.
- While this library handles basic memory pooling and object serialization needs, it can be extended with more complex features like thread-safety or more advanced serialization strategies.
