//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "SecretarialOffice.hpp"
#include "Form.hpp"

#include "ex00.inc"

SecretarialOffice::SecretarialOffice() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[SecretarialOffice]\t" << ENDC << id_ << std::endl;
}

SecretarialOffice::~SecretarialOffice() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[SecretarialOffice]\t" << ENDC << id_ << std::endl;
}

void SecretarialOffice::ArchiveForm(std::shared_ptr<Form>& form) {
  archived_forms_.push_back(form);
  if (DEBUG)
    std::cout << *this << GREEN << " [ARCHIVE FORM]\t" << ENDC << *form << std::endl;
}

std::ostream& operator<<(std::ostream& s, const SecretarialOffice& so) {
  s << BLUE << "[SecretarialOffice]" << ENDC << *(dynamic_cast<const Room*>(&so));
  return (s);
}

