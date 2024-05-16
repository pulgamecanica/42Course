//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Staff.hpp"

#include "ex00.inc"

Staff::Staff(const std::string & name): Person(name) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Staff]\t" << ENDC << name_ << std::endl;
}

Staff::~Staff() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Staff]\t" << ENDC << name_ << std::endl;
}

void Staff::Sign() const {
  std::cout << "Signing" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Staff& staff) {
  s << BLUE << "[Staff]" << ENDC << *(dynamic_cast<const Person*>(&staff));
  return (s);
}

