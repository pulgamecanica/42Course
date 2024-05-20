//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Professor.hpp"
#include "Course.hpp"
#include "Classroom.hpp"
#include "IMediator.hpp"

#include "ex03.inc"

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

void Professor::Teach() {
  if (!mediator_) {
    if (DEBUG)
      std::cout << "Cannot teach without a mediator" << std::endl;
    return ;
  }
  if (GetCurrentCourse() == nullptr || GetCurrentCourse()->IsFinished()) {
    mediator_->Notify(shared_from_this(), Notification::ProfessorRequestNewCourse);
  } else {
    std::shared_ptr<Classroom> classroom = std::dynamic_pointer_cast<Classroom>(GetCurrentRoom());
    if (classroom) {
      current_course_.lock()->GiveClass();
      if (DEBUG)
        std::cout << *this << GREEN << " [TEACH]\t" << ENDC << " at " << *GetCurrentRoom() << std::endl;
      if (current_course_.lock()->LessonsLeft() == 0)
        mediator_->Notify(shared_from_this(), Notification::ProfessorRequestGraduation);
    } else {
      mediator_->Notify(shared_from_this(), Notification::ProfessorRequestNewClassroom);
    }
  }
}

std::shared_ptr<Course> Professor::GetCurrentCourse() const {
  return current_room_.expired() && !current_course_.lock() ? nullptr : current_course_.lock();
}

std::ostream& operator<<(std::ostream& s, const Professor& p) {
  s << BLUE << "[Professor]" << ENDC << *(dynamic_cast<const Staff*>(&p));
  return (s);
}

