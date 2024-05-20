//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "Staff.hpp"
#include "Form.hpp"

Staff::Staff(const std::string & name):
    Person(name) {
    ;
}

Staff::~Staff() {
    ;
}

void Staff::sign(Form* p_form) {
    p_form->sign(this);
}

std::ostream& operator<<(std::ostream& s, const Staff& p) {
    s << GREEN << "[Staff] " << ENDC << *(dynamic_cast<const Person *>(&p));
    return s;
}
