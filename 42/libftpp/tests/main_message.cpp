#include "message.hpp"
#include <string>

void actionate(const Message& msg){
  int doubledValue;
  msg >> doubledValue;
  std::cout << "Received a doubled value: " << doubledValue << std::endl;
};


void actionate2(const Message& msg){
  std::string msgStr;
  msg >> msgStr;
  std::cout << "Received a string value: " << msgStr << std::endl;
};

int main() {
  {
    // Message of type 1 (Int)
    Message message1(1);
    message1 << 42;

    // Message of type 2 (size_t followed by characters)
    Message message2(2);
    std::string str = "Hello";
    message2 << str.size();
    for (char c : str) {
        message2 << c;
    }

    // Message of type 3 (double)
    Message message3(3);
    message3 << 42.42f;

    int value;
    message1 >> value;

    std::string strVal;
    message2 >> strVal;

    float floatVal;
    message3 >> floatVal;


    std::cout << "Message1: " << value << std::endl; 
    std::cout << "Message2: " << strVal << std::endl; 
    std::cout << "Message3: " << floatVal << std::endl; 
  }

  {
    // Message of type 1 (Int)
    Message message1(1);
    message1 << 42;

    // Message of type 2 (size_t followed by characters)
    Message message2(2);
    std::string str = "Hello";
    message2 << str.size();
    for (char c : str) {
        message2 << c;
    }

    actionate(message1);
    actionate2(message2);
  }
  return 0;
}
