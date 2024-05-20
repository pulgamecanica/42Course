//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Professor.hpp"
#include "Course.hpp"

#include "ex02.inc"

Professor::Professor(const std::string & name): Staff(name) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Professor]\t" << ENDC << name_ << std::endl;
}

Professor::~Professor() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Professor]\t" << ENDC << name_ << std::endl;
}

void Professor::SetCourse(std::shared_ptr<Course> course) {
  if (!current_course_.expired()) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " SET COURSE]" << ENDC << *this << "Cannot assign a Course twice or assign a different Course without removing the old one." << std::endl;
  } else {
    current_course_ = course;
    if (DEBUG)
      std::cout << *this << GREEN << " [SET COURSE]\t" << ENDC << *course << std::endl;
  }
}

void Professor::RemoveCourse() {
  current_course_.reset();
  if (DEBUG)
    std::cout << *this << GREEN << " [REMOVE COURSE]\t" << ENDC << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Professor& p) {
  s << BLUE << "[Professor]" << ENDC << *(dynamic_cast<const Staff*>(&p));
  return (s);
}

