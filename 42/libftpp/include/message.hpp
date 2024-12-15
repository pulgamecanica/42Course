/**
* @file message.hpp
* @brief Provides the message implementation between `Server` & `Client`
*/

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "data_buffer.hpp"
#include <stdexcept>
#include <string>

/**
 * @class Message
 * @brief A class for serializing and deserializing messages using `DataBuffer`.
 */
class Message {
public:
  /**
   * @brief Enum to represent the type of message.
   */
  enum class Type {
    Int = 1,
    String = 2,
    Double = 3
  };

  /**
   * @brief Constructor for Message.
   * @param type The type of the message.
   */
  Message(int type);

  /**
   * @brief Serialize data into the message.
   * @tparam T The type of the data to serialize.
   * @param data The data to serialize.
   * @return Reference to the current message for chaining.
   */
  Message& operator<<(const std::string& str);
  
  template <typename T>
  Message& operator<<(const T& data) {
    switch (message_type_) {
      case Type::Int: {
        if constexpr (std::is_same_v<T, int>)
          break ;
        throw std::runtime_error("Message type mismatch: expected Int");
      }
      case Type::String: {
        if constexpr (std::is_same_v<T, size_t> || std::is_same_v<T, std::string> || std::is_same_v<T, char>)
          break ;
        throw std::runtime_error("Message type mismatch: expected std::string");
      }
      case Type::Double: {
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
          break ;
        throw std::runtime_error("Message type mismatch: expected double | float");
      }
      default: {
        throw std::runtime_error("Message type mismatch: invalid type");
      }
    }
    buffer_ << data;
    return *this;
  }

  /**
   * @brief Deserialize data from the message.
   * @tparam T The type of the data to deserialize.
   * @param data The variable to hold deserialized data.
   * @return Reference to the current message for chaining.
   * @throw std::runtime_error if the deserialization fails.
   */
  Message operator>>(std::string& str) const;

  template <typename T>
  Message operator>>(T& data) const {
    if constexpr (std::is_same_v<T, int>) {
      if (message_type_ != Type::Int) {
        throw std::runtime_error("Message type mismatch: expected Int");
      }
    } else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
      if (message_type_ != Type::Double) {
        throw std::runtime_error("Message type mismatch: expected Double");
      }
    }
    int type;
    buffer_ >> type;
    buffer_ >> data;
    return *this;
  }

  /**
   * @brief Returns the type of the message.
   * @return The type of the message.
   */
  int type() const;

  /**
   * @brief Returns the serialized data.
   * @return The serialized data.
   */
  std::vector<uint8_t> getSerializedData() const;

  /**
   * @brief Deserialize a buffer into multiple `Message` objects.
   * @param data The raw data buffer containing one or more serialized messages.
   * @return A vector of deserialized `Message` objects.
   * @throw std::runtime_error if the data is invalid or incomplete.
   */
  static std::vector<Message> deserializeMessages(const std::vector<uint8_t>& data);

private:
  Type message_type_;        ///< The type of the message.
  mutable DataBuffer buffer_;        ///< The data buffer for serialization.
};

#endif // MESSAGE_HPP
