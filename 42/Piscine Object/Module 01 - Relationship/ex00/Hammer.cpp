//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Hammer.hpp"

namespace relationship {
    Hammer::Hammer(): Tool("hammer", 9) {
    }

    Hammer::~Hammer() {
    }

    void Hammer::use() {
        if (uses_ <= 0) {
            std::cout << GREEN << "NO MORE USES\t" << *this << std::endl;
        } else {
            std::cout << GREEN << "USING\t" << *this << "OUCH I hammered my finger :(" << std::endl;
            uses_--;
        }
    }
}
