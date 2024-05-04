//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>

#include "School.inc"
#include "Student.hpp"
#include "Course.hpp"
#include "StudentList.hpp"

Student::Student(const std::string & name):
    Person(name) {
    StudentList::GetInstance()->add(this);
}

Student::~Student() {
    StudentList::GetInstance()->remove(this);
}

void Student::attendClass(ClassRoom * p_classroom) {
    if (current_room_ != nullptr)
        exitClass();
    setRoom(p_classroom);
    if (hasCourse(p_classroom->getCourse()))
        courses_[p_classroom->getCourse()]++;
}

void Student::exitClass() {
    setRoom(nullptr);
}

bool Student::hasCourse(Course * p_course) const {
    return courses_.count(p_course) > 0;
}

void Student::graduate(Course * p_course) {
    if (!hasCourse(p_course)) {
        if (DEBUG)
            std::cout << YELLOW << "[Student] " << RED << "GRADUATE FAIL\t" << ENDC << *this << " is not substribed to " << *p_course << std::endl;
        return;
    }
    courses_.erase(p_course);
    if (DEBUG)
        std::cout << YELLOW << "[Student] " << GREEN << "GRADUATED\t" << ENDC << *this << " from " << *p_course << std::endl;
}

void Student::addCourse(Course * p_course) {
    if (hasCourse(p_course)) {
        if (DEBUG)
            std::cout << YELLOW << "[Student] " << RED << "ADD COURSE FAIL\t" << ENDC << *this << " is already subscribed to " << *p_course << std::endl;
        return ;
    } else if (!p_course->hasStudent(this)) {
        if (DEBUG)
            std::cout << YELLOW << "[Student] " << RED << "ADD COURSE FAIL\t" << ENDC << *this << " must request to subscribe to " << *p_course << std::endl;
        return ;
    }
    courses_[p_course] = 0;
    if (DEBUG)
        std::cout << YELLOW << "[Student] " << GREEN << "ADD COURSE\t" << ENDC << *p_course << " to " << *this << std::endl;
}

int Student::getCourseAttendance(Course * p_course) {
    if (hasCourse(p_course))
        return courses_.at(p_course);
    return 0;
}

void Student::printStudentProgram() const {
    std::cout << *this << YELLOW << " Program (" << courses_.size() << " Courses)" << ENDC << std::endl;
    for (std::map<Course *, int>::const_iterator i = courses_.begin(); i != courses_.end(); ++i)
        std::cout << " - " << BLUE << *i->first << ENDC << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Student& param) {
    s << "Student: " << *(dynamic_cast<const Person *>(&param));
    return (s);
}

