//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Classroom.hpp"

#include "ex00.inc"

Classroom::Classroom() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Classroom]\t" << ENDC << id_ << std::endl;
}

Classroom::~Classroom() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Classroom]\t" << ENDC << id_ << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Classroom& cr) {
  s << BLUE << "[Classroom]" << ENDC << *(dynamic_cast<const Room*>(&cr));
  return (s);
}

