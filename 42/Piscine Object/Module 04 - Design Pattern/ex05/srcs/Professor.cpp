//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Professor.hpp"
#include "Course.hpp"
#include "Classroom.hpp"
#include "StaffRestRoom.hpp"
#include "IMediator.hpp"
#include "RoomList.hpp"

#include "ex04.inc"

Professor::Professor(const std::string & name): Staff(name) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Professor]\t" << ENDC << name_ << std::endl;
}

Professor::~Professor() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Professor]\t" << ENDC << name_ << std::endl;
}

void Professor::Update(enum Event e) {
  switch (e) {
    case Event::RingBell : {
      if (HasClassroom()) {
        (std::dynamic_pointer_cast<Classroom>(GetCurrentRoom()))->RemoveCourse();
        ClearCurrentRoom();
        auto room = RoomList::GetInstance()->getFirst<StaffRestRoom>();
        if (room)
          SetCurrentRoom(room);
      } else {
        // Teach(); // Very confussing, what does "return in class" in the subject mean, return to classroom, return to teach?
        EnterOffice();
      }
    } return ;
    case Event::FoodTruckHorn : {
      if (HasClassroom())
        ClearCurrentRoom();
    }
  }
}

void Professor::EnterOffice() {
  if (NeedCourse()) {
    std::cout << "Must have a course before entering a classroom, classrooms need to have a course from a professor entering" << std::endl;
    return ;
  }
  auto self = shared_from_this();
  auto classroom = std::dynamic_pointer_cast<Classroom>(
    RoomList::GetInstance()->getFirst<Classroom>(
    [self](std::shared_ptr<Classroom> cr) {
        return cr->CanEnter(self);
    })
  );
  if (classroom) {
    SetCurrentRoom(classroom);
    classroom->SetCourse(GetCurrentCourse());
    // Teach(); // Not clear if by entering the room he should give class right away..
  } else {
    mediator_->Notify(shared_from_this(), Notification::ProfessorRequestNewClassroom);
  }
  if (DEBUG)
    std::cout << *this << GREEN << " [ENTER OFFICE]\t" << ENDC << std::endl;
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
  if (NeedCourse()) {
    mediator_->Notify(shared_from_this(), Notification::ProfessorRequestNewCourse);
  }
  if (!HasClassroom()) {
    EnterOffice();
  }
  if (!NeedCourse() && HasClassroom()) {
    current_course_.lock()->GiveClass();
    if (DEBUG)
      std::cout << *this << GREEN << " [TEACH]\t" << ENDC << " at " << *GetCurrentRoom() << std::endl;
    if (GetCurrentCourse()->LessonsLeft() == 0)
      mediator_->Notify(shared_from_this(), Notification::ProfessorRequestGraduation);
  }
}

std::shared_ptr<Course> Professor::GetCurrentCourse() const {
  return current_room_.expired() && !current_course_.lock() ? nullptr : current_course_.lock();
}

bool Professor::NeedCourse() const {
  return GetCurrentCourse() == nullptr || GetCurrentCourse()->IsFinished();
}

bool Professor::HasClassroom() const {
  return std::dynamic_pointer_cast<Classroom>(GetCurrentRoom()) != nullptr;
}

std::ostream& operator<<(std::ostream& s, const Professor& p) {
  s << BLUE << "[Professor]" << ENDC << *(dynamic_cast<const Staff*>(&p));
  return (s);
}

