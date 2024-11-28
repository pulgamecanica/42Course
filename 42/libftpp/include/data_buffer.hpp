/**
 * @file data_buffer.hpp
 * @brief Header file for the `DataBuffer` class, a polymorphic container for serialized objects.
 */

#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <cstring>

/**
 * @class DataBuffer
 * @brief A polymorphic container for serializing and deserializing objects into byte format.
 * 
 * The `DataBuffer` class allows storing and retrieving objects in a byte format. It handles
 * serialization (storing objects in the buffer) and deserialization (retrieving objects from the buffer)
 * using the `<<` and `>>` operators.
 * 
 * The class uses an internal byte buffer to store serialized data and provides convenient methods
 * for storing and loading data of various types.
 * 
 * # Example Usage:
 * 
 * ## Serializing and Deserializing Basic Types:
 * 
 * ```cpp
 * DataBuffer buffer;
 * int number = 42;
 * buffer << number;
 * 
 * int deserializedNumber;
 * buffer >> deserializedNumber;
 * std::cout << "Deserialized number: " << deserializedNumber << std::endl;  // Output: 42
 * ```
 * 
 * ## Serializing and Deserializing Custom Objects:
 * 
 * ```cpp
 * class TestObject {
 * public:
 *     int x;
 *     std::string y;
 * 
 *     friend DataBuffer& operator<<(DataBuffer& p_buffer, const TestObject& p_object) {
 *         p_buffer << p_object.x << p_object.y;
 *         return p_buffer;
 *     }
 * 
 *     friend DataBuffer& operator>>(DataBuffer& p_buffer, TestObject& p_object) {
 *         p_buffer >> p_object.x >> p_object.y;
 *         return p_buffer;
 *     }
 * };
 * 
 * DataBuffer buffer;
 * TestObject obj1{42, "Hello"};
 * buffer << obj1;
 * 
 * TestObject deserializedObj;
 * buffer >> deserializedObj;
 * std::cout << "Deserialized object: x = " << deserializedObj.x << ", y = " << deserializedObj.y << std::endl;
 * ```
 */
class DataBuffer {
public:
  /**
   * @brief Default constructor for `DataBuffer`.
   * 
   * Initializes an empty internal buffer to hold the serialized data.
   */
  DataBuffer();

  /**
   * @brief Constructor for `DataBuffer`.
   * 
   * Initializes a DataBuffer with the given data.
   */
  DataBuffer(const std::vector<uint8_t>& buffer);

  /**
   * @brief Serialize a `std::string` object and store it in the buffer.
   * 
   * This operator serializes the `std::string` object `str` by first storing its length
   * followed by the characters of the string.
   * 
   * @param str The `std::string` to be serialized and stored in the buffer.
   * @return A reference to the current `DataBuffer` object for method chaining.
   */
  DataBuffer& operator<<(const std::string& str);

  /**
   * @brief Serialize an object and store it in the buffer.
   * 
   * This operator appends the serialized representation of the object `obj` to the internal buffer.
   * The object is first converted into a byte format and then inserted into the buffer.
   * 
   * @tparam T The type of the object being serialized.
   * @param obj The object to be serialized and stored in the buffer.
   * @return A reference to the current `DataBuffer` object for method chaining.
   * 
   * @note The type `T` must be **trivially copyable**. This is enforced via a static assertion
   * that will cause a compile-time error if the type `T` is not trivially copyable. A trivially 
   * copyable type is one that can be safely copied byte-by-byte (such as primitive types or simple
   * structs without virtual functions or non-trivial destructors).
   * https://en.cppreference.com/w/cpp/types/is_trivially_copyable
   */
  template <typename T>
  DataBuffer& operator<<(const T& obj) {
    static_assert(std::is_trivially_copyable<T>::value, "Type must be trivially copyable.");
    const uint8_t* data = reinterpret_cast<const uint8_t*>(&obj);
    buffer_.insert(buffer_.end(), data, data + sizeof(T));
    return *this;
  }

  /**
   * @brief Serialize a `std::string` object and store it in the buffer.
   * 
   * This operator serializes the `std::string` object `str` by first storing its length
   * followed by the characters of the string.
   * 
   * @param str The `std::string` to be serialized and stored in the buffer.
   * @return A reference to the current `DataBuffer` object for method chaining.
   */
  DataBuffer& operator>>(std::string& str);

  /**
   * @brief Deserialize an object from the buffer.
   * 
   * This operator retrieves data from the internal buffer and deserializes it into the object `obj`.
   * If the buffer contains insufficient data for the requested object, an exception is thrown.
   * 
   * @tparam T The type of the object being deserialized.
   * @param obj The object to be deserialized and populated with data from the buffer.
   * @return A reference to the current `DataBuffer` object for method chaining.
   * 
   * @throws std::out_of_range If the buffer does not contain enough data for the object.
   * 
   * @note The type `T` must be **trivially copyable**. This is enforced via a static assertion
   * that will cause a compile-time error if the type `T` is not trivially copyable. A trivially 
   * copyable type is one that can be safely copied byte-by-byte (such as primitive types or simple
   * structs without virtual functions or non-trivial destructors).
   * https://en.cppreference.com/w/cpp/types/is_trivially_copyable
   */
  template <typename T>
  DataBuffer& operator>>(T& obj) {
    static_assert(std::is_trivially_copyable<T>::value, "Type must be trivially copyable.");
    if (buffer_.empty()) {
        throw std::out_of_range("Buffer is empty.");
    }

    size_t dataSize = sizeof(T);
    if (buffer_.size() < dataSize) {
        throw std::out_of_range("Buffer does not contain enough data.");
    }

    std::memcpy(&obj, buffer_.data(), dataSize);
    buffer_.erase(buffer_.begin(), buffer_.begin() + dataSize); // Remove the data from the buffer
    return *this;
  }


  /**
   * @brief Retrieve the raw byte data stored in the buffer.
   * 
   * This method returns a reference to the internal byte buffer containing the serialized data.
   * 
   * @return A constant reference to the internal buffer.
   */
  const std::vector<uint8_t>& getData() const;

private:
  std::vector<uint8_t> buffer_; ///< The internal buffer holding the serialized data.
};

#endif // DATA_BUFFER_HPP
