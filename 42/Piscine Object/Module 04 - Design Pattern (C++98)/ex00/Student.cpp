//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Student.hpp"
#include "Course.hpp"

Student::Student(const std::string & name):
    Person(name), current_classroom_(nullptr) {
    ;
}

Student::~Student() {
    // std::cout << "Student" << " destroyed" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Student& param) {
    s << "Student: " << *(dynamic_cast<const Person *>(&param));
    return (s);
}

