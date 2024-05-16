//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "Staff.hpp"
#include "StaffList.hpp"

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
    std::cout << p_form << "signed" << std::endl;
    p_form->execute();
}

std::ostream& operator<<(std::ostream& s, const Staff& p) {
    s << GREEN << "[Staff] " << ENDC << *(dynamic_cast<const Person *>(&p));
    return s;
}
