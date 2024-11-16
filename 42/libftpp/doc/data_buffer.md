# DataBuffer - Summary

This section provides an overview of the `DataBuffer` class, which is responsible for serializing and deserializing objects in byte format. The `DataBuffer` class is crucial for managing the storage and retrieval of objects in a raw, byte-oriented format.

## Purpose

The `DataBuffer` class allows for efficient storage and retrieval of objects in a byte format. It supports the serialization of objects to a byte buffer and deserialization from it, providing an easy-to-use interface via `<<` and `>>` operators.

## Key Features:
- **Polymorphic serialization**: The class can serialize a wide range of objects, including basic types and custom classes, to a byte buffer.
- **Deserialization**: The class also supports deserialization, reading objects from the byte buffer back into their original form.
- **Operator overloading**: The `<<` and `>>` operators are overloaded to handle both serialization and deserialization for various types.
- **Buffer management**: The internal buffer is dynamically resized as needed to store serialized data.

## Methods:
- **operator<< (const T&)**: Serializes an object of type `T` and appends it to the buffer.
- **operator>> (T&)**: Deserializes an object of type `T` from the buffer.
- **getData**: Retrieves the raw byte data stored in the buffer.

## Example Usage:

### Serializing and Deserializing Basic Types

```cpp
DataBuffer buffer;
int number = 42;

// Serialize an integer
buffer << number;

// Deserialize the integer
int deserializedNumber;
buffer >> deserializedNumber;
std::cout << "Deserialized number: " << deserializedNumber << std::endl;  // Output: 42
```

### Serializing and Deserializing Custom Objects
```
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

DataBuffer buffer;
TestObject obj1{42, "Hello"};

// Serialize a custom object
buffer << obj1;

// Deserialize the object
TestObject deserializedObj;
buffer >> deserializedObj;
std::cout << "Deserialized object: x = " << deserializedObj.x << ", y = " << deserializedObj.y << std::endl;
```