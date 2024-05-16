//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>

#include "ex00.inc"
#include "Student.hpp"
#include "Professor.hpp"
#include "Course.hpp"

namespace SpringfieldElementary {
Course::Course(std::string p_name, int p_capacity, int p_total_lessons_):
    name_(p_name), total_lessons_(p_total_lessons_), capacity_(p_capacity) {
        if (DEBUG)
            std::cout << GREEN << "[NEW]" << YELLOW << "[Course]\t" << ENDC << name_ << std::endl;
}

Course::~Course() {
    if (DEBUG)
        std::cout << RED << "[DESTROY]" << YELLOW << "[Course]\t" << ENDC << name_ << std::endl;
}

void Course::assign(std::shared_ptr<Professor> p_professor) {
    if (hasProfessor(p_professor)) {
        if (DEBUG)
            std::cout << RED << "[ASSIGN FAIL]" << YELLOW << "[Course]\t" << ENDC << *p_professor << " is already the responsible" << std::endl;
        return;
    }
    responsible_ = p_professor;
    if (DEBUG)
        std::cout << GREEN << "[ASSIGN]" << YELLOW << "[Course]\t" << ENDC << *p_professor << std::endl;
}

void Course::subscribe(std::shared_ptr<Student> p_student) {
    if (hasStudent(p_student)) {
        if (DEBUG)
            std::cout << RED << "[SUBSCRIBE FAIL]" << YELLOW << "[Course]\t" << ENDC << *p_student << " is already subscribed" << std::endl;
        return;
    }
    students_.emplace_back(p_student);
    if (DEBUG)
        std::cout << GREEN << "[SUBSCRIBE]" << YELLOW << "[Course]\t" << ENDC << *p_student << std::endl;
}

void Course::unsubscribe(std::shared_ptr<Student> p_student) {
    auto it = std::find_if(students_.begin(), students_.end(),
        [p_student](const std::weak_ptr<Student>& weak_ptr) {
            return !weak_ptr.expired() && weak_ptr.lock() == p_student;
        });
    if (it != students_.end()) {
        students_.erase(it);
        std::cout << GREEN << "[UNSUBSCRIBE]" << YELLOW << "[Course]\t" << ENDC << *p_student << " from " << *this << std::endl;
    } else {
        if (DEBUG)
            std::cout << RED << "[UNSUBSCRIBE FAIL]" << YELLOW << "[Course]\t" << ENDC << *p_student << " is not subscribed to " << *this << std::endl;
    }
}

std::ostream& Course::loadCourseInfo(std::ostream& s) const {
    s << BLUE << name_ << ENDC;
    if (responsible_)
        s << " by " << *responsible_ << ", ";
    else
        s << " no responsible, ";
    s << students_.size() << "/" << capacity_ << " students, total lessons: " << total_lessons_;
    return s;
}

bool Course::hasStudent(std::shared_ptr<Student> p_student) const {
    return std::any_of(students_.begin(), students_.end(),
        [p_student](const std::weak_ptr<Student>& weak_ptr) {
            return !weak_ptr.expired() && weak_ptr.lock() == p_student;
        });
}

bool Course::hasProfessor(std::shared_ptr<Professor> p_professor) const {
    return p_professor == responsible_;
}

std::ostream& operator<<(std::ostream& s, const Course& c) {
    return c.loadCourseInfo(s);
}

}
