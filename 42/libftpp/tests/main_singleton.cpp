#include <iostream>
#include "singleton.hpp"

class MyClass {
public:
	MyClass(int value)
	{
		std::cout << "MyClass constructor, with value [" << value << "]" << std::endl;
	}

    void printMessage() {
        std::cout << "Hello from MyClass" << std::endl;
    }
};

int main() {
    try
    {
        // This should throw an exception as instance is not yet created
        Singleton<MyClass>::instance();
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl; // Output: "Exception: Instance not yet created"
    }

    Singleton<MyClass>::instantiate(42); // Setting up the instance

    Singleton<MyClass>::instance()->printMessage(); // Output: "Hello from MyClass"

    try
    {
        // This should throw an exception as instance is already created
        Singleton<MyClass>::instantiate(100);
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl; // Output: "Exception: Instance already created"
    }

    return 0;
}
