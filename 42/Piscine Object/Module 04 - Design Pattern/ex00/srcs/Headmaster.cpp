//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Headmaster.hpp"
#include "Form.hpp"

#include "ex00.inc"

Headmaster::Headmaster(const std::string& name)
  : Staff(name) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Headmaster]\t" << ENDC << name_ << std::endl;
}

Headmaster::~Headmaster() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Headmaster]\t" << ENDC << name_ << std::endl;
}

void Headmaster::ReceiveForm(std::shared_ptr<Form>& form) {
  // forms_to_validate_.push_back(form);
  // if the form is in the forms to validate, sign and execute
  Sign(form);
  if (form->Errors() == 0)
    form->Execute();
  if (DEBUG)
    std::cout << *this << GREEN << " [RECEIVE FORM]\t" << ENDC << *form << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Headmaster& hm) {
  s << BLUE << "[Headmaster]" << ENDC << *(dynamic_cast<const Staff*>(&hm));
  return (s);
}

