//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Shovel.hpp"

namespace relationship {
    Shovel::Shovel(): Tool("shovel", 42) {
    }

    Shovel::~Shovel() {
    }

    void Shovel::use() {
        if (uses_ <= 0) {
            std::cout << GREEN << "NO MORE USES\t" << *this << std::endl;
        } else {
            std::cout << GREEN << "USING\t" << *this << " digging deeper to find the treassure!" << std::endl;
            uses_--;
        }
    }
}
