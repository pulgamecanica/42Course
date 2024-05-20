//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "Staff.hpp"
#include "StaffList.hpp"
#include "Form.hpp"

Staff::Staff(const std::string & name):
    Person(name) {
    // AWESOME, even if we add the Staff pointer, we can use polymorphism to convert to a derived class!
    StaffList::GetInstance()->add(this);
    ;
}

Staff::~Staff() {
    StaffList::GetInstance()->remove(this);
}

void Staff::sign(Form* p_form) {
    p_form->sign(this);
}

std::ostream& operator<<(std::ostream& s, const Staff& p) {
    s << GREEN << "[Staff] " << ENDC << *(dynamic_cast<const Person *>(&p));
    return s;
}
