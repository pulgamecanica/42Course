#include "data_buffer.hpp" // Assuming your DataBuffer is defined in this header
#include <iostream>
#include <string>
#include <exception>

class TestObject {
public:
    int x;
    std::string y;

    friend DataBuffer& operator<<(DataBuffer& p_buffer, const TestObject& p_object) {
        p_buffer << p_object.x << p_object.y;
        return p_buffer;
    }

    friend DataBuffer& operator>>(DataBuffer& p_buffer, TestObject& p_object) {
        p_buffer >> p_object.x >> p_object.y;
        return p_buffer;
    }
};

int main() {
    DataBuffer myBuffer;

    TestObject obj1;
    obj1.x = 42;
    obj1.y = "Hello";

    TestObject obj2;
    obj2.x = 99;
    obj2.y = "World";

    myBuffer << obj1 << obj2;

    TestObject deserializedObj1, deserializedObj2, deserializedObj3;

    // This should work as expected
    try {
        myBuffer >> deserializedObj1 >> deserializedObj2;
        std::cout << "Deserialized obj1: x = " << deserializedObj1.x << ", y = " << deserializedObj1.y << std::endl;
        std::cout << "Deserialized obj2: x = " << deserializedObj2.x << ", y = " << deserializedObj2.y << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    // This should throw an exception because there are no more objects to deserialize
    try {
        myBuffer >> deserializedObj3;
        std::cout << "Deserialized obj3: x = " << deserializedObj3.x << ", y = " << deserializedObj3.y << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;  // This line should be executed
    }
    return 0;
}
