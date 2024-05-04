//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Courtyard.hpp"

Courtyard::Courtyard() {
    ;
}

Courtyard::~Courtyard() {
    // std::cout << "Courtyard" << " destroyed" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Courtyard& cy) {
    s << "Courtyard: " << *(dynamic_cast<const Room *>(&cy));
    return (s);
}

