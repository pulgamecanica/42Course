//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Base.hpp"

Base::Base(int num): num_(num) {
    std::cout << "Base " << num << " created" << std::endl;
}

Base::~Base() {
    std::cout << "Base" << " destroyed" << std::endl;
}

