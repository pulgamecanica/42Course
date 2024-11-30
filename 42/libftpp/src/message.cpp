#include "message.hpp"

Message::Message(int type) 
  : message_type_(static_cast<Type>(type)) {
  if (type < 1 || type > 3) {
    throw std::invalid_argument("Invalid message type");
  }
  buffer_ << type;
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

Message Message::operator>>(std::string& str) const {
  if (message_type_ != Type::String) {
    throw std::runtime_error("Message type mismatch: expected String");
  }
  int type;
  buffer_ >> type;
  buffer_ >> str; // My buffer already does all the job
  return *this;
}

std::vector<uint8_t> Message::getSerializedData() const {
  return buffer_.getData();
}

std::vector<Message> Message::deserializeMessages(const std::vector<uint8_t>& data) {
  std::vector<Message> messages;
  DataBuffer data_buffer(data);

  if (data.size() < sizeof(int)) {
    throw std::runtime_error("Buffer is too small to extract the type!");
  }

  while (42) {
    int type;
    try {
      data_buffer >> type;
      if (type < 1 || type > 3) {
        throw std::invalid_argument("Invalid message type");
      }
    } catch ( std::out_of_range &e ) {
      break ;
    } catch ( ... ) {
      continue ;
    }
    Message msg(type);
    switch (static_cast<Type>(type)) {
      case Type::Int: {
        int val;
        data_buffer >> val;
        msg << val;
        break ;
      }
      case Type::String: {
        std::string val;
        data_buffer >> val;
        msg << val;
        break ;
      }
      case Type::Double: {
        double val;
        data_buffer >> val;
        msg << val;
        break ;
      }
    }
    messages.emplace_back(msg);
  }
  if (messages.empty()) {
    throw std::runtime_error("Couldn't deserialize any messages");
  }
  return messages;
}

