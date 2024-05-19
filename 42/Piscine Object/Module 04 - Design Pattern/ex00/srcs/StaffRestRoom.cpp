//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "StaffRestRoom.hpp"

#include "ex00.inc"

StaffRestRoom::StaffRestRoom() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[StaffRestRoom]\t" << ENDC << id_ << std::endl;
}

StaffRestRoom::~StaffRestRoom() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[StaffRestRoom]\t" << ENDC << id_ << std::endl;
}

std::ostream& operator<<(std::ostream& s, const StaffRestRoom& sr) {
  s << BLUE << "[StaffRestRoom]" << ENDC << *(dynamic_cast<const Room*>(&sr));
  return (s);
}

