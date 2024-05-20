//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "HeadmasterOffice.hpp"
#include "Headmaster.hpp"
#include "Secretary.hpp"

#include "ex03.inc"

HeadmasterOffice::HeadmasterOffice() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[HeadmasterOffice]\t" << ENDC << id_ << std::endl;
}

HeadmasterOffice::~HeadmasterOffice() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[HeadmasterOffice]\t" << ENDC << id_ << std::endl;
}

bool HeadmasterOffice::CanEnter(std::shared_ptr<Person> person) {
  return (
    std::dynamic_pointer_cast<Secretary>(person) != nullptr ||
    std::dynamic_pointer_cast<Headmaster>(person) != nullptr);
}

std::ostream& operator<<(std::ostream& s, const HeadmasterOffice& ho) {
  s << BLUE << "[HeadmasterOffice]" << ENDC << *(dynamic_cast<const Room*>(&ho));
  return (s);
}

