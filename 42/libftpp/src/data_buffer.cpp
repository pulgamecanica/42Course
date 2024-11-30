#include "data_buffer.hpp"

DataBuffer::DataBuffer(): buffer_() {
    // Initializes an empty buffer
}

DataBuffer::DataBuffer(const std::vector<uint8_t>& buffer): buffer_(buffer) {}

const std::vector<uint8_t>& DataBuffer::getData() const {
    return buffer_;
}

DataBuffer& DataBuffer::operator<<(const std::string& str) {
    // Serialize the length of the string
    size_t length = str.size();
    *this << length; // This will serialize the length first
    
    // Serialize the string data (its characters)
    const uint8_t* data = reinterpret_cast<const uint8_t*>(str.c_str());
    buffer_.insert(buffer_.end(), data, data + length);
    return *this;
}

DataBuffer& DataBuffer::operator>>(std::string& str) {
    // Deserialize the length of the string first
    size_t length = 0;
    *this >> length; // This will deserialize the length first

    // Deserialize the string data (its characters)
    if (buffer_.size() < length) {
        throw std::out_of_range("Buffer does not contain enough data for the string.");
    }

    str.resize(length);
    std::memcpy(&str[0], buffer_.data(), length);
    buffer_.erase(buffer_.begin(), buffer_.begin() + length);
    return *this;
}