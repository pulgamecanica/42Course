//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>

#include "ex00.inc"
#include "Student.hpp"
#include "Course.hpp"

namespace SpringfieldElementary {

Student::~Student() {
    for (const auto& weak_ptr : courses_)
        if (!weak_ptr.expired())
            weak_ptr.lock()->unsubscribe(shared_from_this());
    courses_.clear();
}

// void Student::attendClass(Classroom* p_classroom) {
//     setRoom(p_classroom);
// }

void Student::exitClass() {
    setRoom(nullptr);
}

std::shared_ptr<Student> Student::shared_from_this() {
    return std::static_pointer_cast<Student>(Person::shared_from_this());
}

void Student::addCourse(std::shared_ptr<Course> p_course) {
    if (hasCourse(p_course)) {
        if (DEBUG)
            std::cout << RED << "[ADD COURSE FAIL]" << YELLOW << "[Student]\t" << ENDC << *this << " is already subscribed to " << *p_course << std::endl;
    } else {
        p_course->subscribe(shared_from_this());
        courses_.emplace_back(p_course);
        std::cout << GREEN << "[ADD COURSE]" << YELLOW << "[Student]\t" << ENDC << *p_course << std::endl;
    }
}

void Student::graduate(std::shared_ptr<Course> p_course) {
    auto it = std::find_if(courses_.begin(), courses_.end(),
        [p_course](const std::weak_ptr<Course>& weak_ptr) {
            return !weak_ptr.expired() && weak_ptr.lock() == p_course;
        });
    if (it != courses_.end()) {
        courses_.erase(it);
        std::cout << GREEN << "[GRADUATE]" << YELLOW << "[Student]\t" << ENDC << *this << std::endl;
    } else {
        if (DEBUG)
            std::cout << RED << "[GRADUATE FAIL]" << YELLOW << "[Student]\t" << ENDC << *this << " is not subscribed to" << p_course << std::endl;
    }
}

bool Student::hasCourse(std::shared_ptr<Course> p_course) const {
    return std::any_of(courses_.begin(), courses_.end(),
        [p_course](const std::weak_ptr<Course>& weak_ptr) {
            return !weak_ptr.expired() && weak_ptr.lock() == p_course;
        });
}

std::ostream& operator<<(std::ostream& s, const Student& st) {
    s << BLUE << "[Student] " << ENDC << *(dynamic_cast<const Person *>(&st));
    return s;
}
}
