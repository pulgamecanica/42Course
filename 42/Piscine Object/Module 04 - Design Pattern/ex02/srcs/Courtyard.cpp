//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Courtyard.hpp"

#include "ex02.inc"

Courtyard::Courtyard() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Courtyard]\t" << ENDC << id_ << std::endl;
}

Courtyard::~Courtyard() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Courtyard]\t" << ENDC << id_ << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Courtyard& c) {
  s << BLUE << "[Courtyard]" << ENDC << *(dynamic_cast<const Room*>(&c));
  return (s);
}

