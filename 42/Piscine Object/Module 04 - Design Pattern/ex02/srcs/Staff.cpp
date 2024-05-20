//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Staff.hpp"
#include "Form.hpp"

#include "ex02.inc"

Staff::Staff(const std::string & name): Person(name) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Staff]\t" << ENDC << name_ << std::endl;
}

Staff::~Staff() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Staff]\t" << ENDC << name_ << std::endl;
}

void Staff::Sign(std::shared_ptr<Form> form) {
  if (form->IsValid()) {
    form->Signature(name_);
    if (DEBUG)
      std::cout << *this << GREEN << " [SIGN]\t" << ENDC << *form << std::endl;
  } else {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " SIGN]" << ENDC << *this << " cannot sign " << *form << " : " << RED << form->Errors() << " Errors" << ENDC << std::endl;
    for (auto e: form->ErrorsList())
      std::cout << " x " << RED << e << ENDC << std::endl;
  }
}

std::ostream& operator<<(std::ostream& s, const Staff& staff) {
  s << BLUE << "[Staff]" << ENDC << *(dynamic_cast<const Person*>(&staff));
  return (s);
}

