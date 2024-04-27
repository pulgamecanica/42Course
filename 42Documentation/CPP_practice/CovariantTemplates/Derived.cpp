//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Derived.hpp"

Derived::Derived(const std::string & name): Base(42), name_(name) {
    std::cout << "Derived " << name << " created" << std::endl;
}

Derived::~Derived() {
    std::cout << "Derived" << " destroyed" << std::endl;
}

