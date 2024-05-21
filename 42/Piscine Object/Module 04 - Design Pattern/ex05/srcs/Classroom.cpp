//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Classroom.hpp"
#include "Course.hpp"
#include "Professor.hpp"
#include "Student.hpp"

#include "ex04.inc"

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

/*
 * Professors are allowed to enter their course classrooms
 * Or a classroom without a course
 * Students can enter any classroom
 */
bool Classroom::CanEnter(std::shared_ptr<Person> person) {
  if (std::dynamic_pointer_cast<Professor>(person)) {
    if (IsFree())
      return true;
    return HasCourse(std::dynamic_pointer_cast<Professor>(person)->GetCurrentCourse());
  }
  return (std::dynamic_pointer_cast<Student>(person) != nullptr);
}

bool Classroom::IsFree() const {
  return current_course_.expired();
}

bool Classroom::HasCourse(std::shared_ptr<Course> course) const {
  return course == current_course_.lock();
}

std::ostream& operator<<(std::ostream& s, const Classroom& cr) {
  s << BLUE << "[Classroom]" << ENDC << *(dynamic_cast<const Room*>(&cr));
  return (s);
}
