#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


int main(void) {
    std::cout << "Good bye cruel world!" << std::endl;
    std::ifstream t("token");
    std::stringstream buffer;
    buffer << t.rdbuf();

    std::cout << "This is the content of token (" << buffer.str().length() << ") " << buffer.str() << std::endl;
    std::cout << "Reversed code:" << std::endl;
    for (int i = 0; i < buffer.str().length(); ++i) {
        std::cout << (char)(buffer.str()[i] - i); 
    }
    std::cout << std::endl << "Bye" << std::endl;
    return 1;
}