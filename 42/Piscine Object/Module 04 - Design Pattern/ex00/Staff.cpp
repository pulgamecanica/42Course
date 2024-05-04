//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "Staff.hpp"

Staff::Staff(const std::string & name):
    Person(name) {
    ;
}

Staff::~Staff() {
    // std::cout << "Staff" << " destroyed" << std::endl;
}

void Staff::sign(Form* p_form) {
    std::cout << p_form << "signed" << std::endl;
    p_form->execute();
}

std::ostream& operator<<(std::ostream& s, const Staff& p) {
    s << GREEN << "[Staff] " << ENDC << *(dynamic_cast<const Person *>(&p));
    return s;
}
