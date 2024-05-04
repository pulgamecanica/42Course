//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Student.hpp"
#include "Course.hpp"
#include "StudentList.hpp"

Student::Student(const std::string & name):
    Person(name), current_classroom_(nullptr) {
    StudentList::GetInstance()->add(this);
}

Student::~Student() {
    StudentList::GetInstance()->remove(this);
}

std::ostream& operator<<(std::ostream& s, const Student& param) {
    s << "Student: " << *(dynamic_cast<const Person *>(&param));
    return (s);
}

