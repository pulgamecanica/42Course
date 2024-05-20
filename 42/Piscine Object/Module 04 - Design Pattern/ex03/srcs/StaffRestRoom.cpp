//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "StaffRestRoom.hpp"
#include "Staff.hpp"

#include "ex03.inc"

StaffRestRoom::StaffRestRoom() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[StaffRestRoom]\t" << ENDC << id_ << std::endl;
}

StaffRestRoom::~StaffRestRoom() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[StaffRestRoom]\t" << ENDC << id_ << std::endl;
}

bool StaffRestRoom::CanEnter(std::shared_ptr<Person> person) {
  return (
    std::dynamic_pointer_cast<Staff>(person) != nullptr);
}

std::ostream& operator<<(std::ostream& s, const StaffRestRoom& sr) {
  s << BLUE << "[StaffRestRoom]" << ENDC << *(dynamic_cast<const Room*>(&sr));
  return (s);
}

