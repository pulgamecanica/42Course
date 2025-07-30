// main.cpp

#include <iostream>
#include <string>
#include <ctime>

void run_vector_compliance_tests();

void print_header(const std::string& container_name) {
    std::cout << "\n==========================" << std::endl;
    std::cout << "Testing: " << container_name << std::endl;
    std::cout << "==========================\n" << std::endl;
}

int main() {
    print_header("Vector");
    run_vector_compliance_tests();
    return 0;
}
