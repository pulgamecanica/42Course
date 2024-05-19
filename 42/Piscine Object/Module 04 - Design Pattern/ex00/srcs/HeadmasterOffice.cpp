//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "HeadmasterOffice.hpp"

#include "ex00.inc"

HeadmasterOffice::HeadmasterOffice() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[HeadmasterOffice]\t" << ENDC << id_ << std::endl;
}

HeadmasterOffice::~HeadmasterOffice() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[HeadmasterOffice]\t" << ENDC << id_ << std::endl;
}

std::ostream& operator<<(std::ostream& s, const HeadmasterOffice& ho) {
  s << BLUE << "[HeadmasterOffice]" << ENDC << *(dynamic_cast<const Room*>(&ho));
  return (s);
}

