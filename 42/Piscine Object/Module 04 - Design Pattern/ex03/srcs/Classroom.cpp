//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Classroom.hpp"
#include "Course.hpp"
#include "Staff.hpp"
#include "Student.hpp"

#include "ex03.inc"

Classroom::Classroom() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Classroom]\t" << ENDC << id_ << std::endl;
}

Classroom::~Classroom() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Classroom]\t" << ENDC << id_ << std::endl;
}

void Classroom::SetCourse(std::shared_ptr<Course> course) {
  if (!current_course_.expired())
    RemoveCourse();
  current_course_ = course;
  if (DEBUG)
    std::cout << *this << GREEN << " [SET COURSE]\t" << ENDC << *course << std::endl;
}

void Classroom::RemoveCourse() {
  current_course_.reset();
  if (DEBUG)
    std::cout << *this << GREEN << " [REMOVE COURSE]\t" << ENDC << std::endl;
}

bool Classroom::CanEnter(std::shared_ptr<Person> person) {
  return (
    std::dynamic_pointer_cast<Staff>(person) != nullptr ||
    std::dynamic_pointer_cast<Student>(person) != nullptr);
}

std::ostream& operator<<(std::ostream& s, const Classroom& cr) {
  s << BLUE << "[Classroom]" << ENDC << *(dynamic_cast<const Room*>(&cr));
  return (s);
}
