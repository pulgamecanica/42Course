//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Professor.hpp"

#include "ex00.inc"

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
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " ASSIGN]" << ENDC << *this << "Cannot assign a Course twice or assign a different Course without removing the old one." << std::endl;
    return;
  }
  current_course_ = course;
}

void Professor::RemoveCourse() {
  current_course_.reset();
}

std::ostream& operator<<(std::ostream& s, const Professor& p) {
    s << BLUE << "[Professor]" << ENDC << *(dynamic_cast<const Staff*>(&p));
    return (s);
}

