#include "message.hpp"

Message::Message(int type) 
  : message_type_(static_cast<Type>(type)) {
  if (type < 1 || type > 3) {
    throw std::invalid_argument("Invalid message type");
  }
}

int Message::type() const {
  return static_cast<int>(message_type_);
}

Message& Message::operator<<(const std::string& str) {
  if (message_type_ != Type::String) {
    throw std::runtime_error("Message type mismatch: expected String");
  }
  buffer_ << str; // My buffer already does all the job, size_t + str
  return *this;
}

Message& Message::operator<<(const std::vector<uint8_t>& data) {
  buffer_ = DataBuffer(data);
  return *this;
}

Message Message::operator>>(std::string& str) const {
  if (message_type_ != Type::String) {
    throw std::runtime_error("Message type mismatch: expected String");
  }
  buffer_ >> str; // My buffer already does all the job
  return *this;
}

std::vector<uint8_t> Message::getSerializedData() const {
  return buffer_.getData();
}