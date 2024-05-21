//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Student.hpp"
#include "Course.hpp"
#include "Classroom.hpp"
#include "Professor.hpp"
#include "IMediator.hpp"
#include "RoomList.hpp"
#include "Courtyard.hpp"

#include <algorithm>
#include "ex04.inc"

Student::Student(const std::string & name): Person(name) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Student]\t" << ENDC << name_ << std::endl;
}

Student::~Student() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Student]\t" << ENDC << name_ << std::endl;
}

void Student::Update(enum Event e) {
  switch (e) {
    case Event::RingBell : {
      if (InClass()) {
        ExitClass();
        auto room = RoomList::GetInstance()->getFirst<Courtyard>();
        if (room)
          SetCurrentRoom(room);
      } else {
        AttendClass();
      }
    } return ;
  }
}

void Student::AddCourse(std::shared_ptr<Course> course) {
  for (const auto& c : courses_) {
    if (c.lock() == course) {
      std::cout << RED << "[" << YELLOW << "Warning" << RED << " ADD COURSE]" << ENDC << *this << "Cannot add a Course twice." << std::endl;
      return;
    }
  }
  if (DEBUG)
      std::cout << *this << GREEN << " [ADD COURSE]\t" << ENDC << *course << std::endl;
  courses_.push_back(course);
}

void Student::Graduate(std::shared_ptr<Course> course) {
  auto it = std::remove_if(courses_.begin(), courses_.end(),
                           [&course](const std::weak_ptr<Course>& c) {
                             return c.lock() == course;
                           });
  if (it == courses_.end()) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " GRADUATE]" << ENDC << *this << "Removing an unexisting Course." << std::endl;
  } else {
    courses_.erase(it, courses_.end());
    if (DEBUG)
      std::cout << *this << GREEN << " [GRADUATE]\t" << ENDC << *course << std::endl;
  }
}

void Student::AttendClass() {
  if (!mediator_) {
    if (DEBUG)
      std::cout << "Cannot attend classes without a mediator" << std::endl;
    return ;
  }
  if (courses_.size() == 0)
    mediator_->Notify(shared_from_this(), Notification::StudentRequestToJoinCourse);
  for (auto course: courses_) { // This makes no sense... but okay, let's follow datas.hpp... hugh.
    if (course.expired())
      continue ;
    std::shared_ptr<Professor> prof = course.lock()->GetResponsible();
    if (prof) {
      std::shared_ptr<Classroom> classroom = std::dynamic_pointer_cast<Classroom>(prof->GetCurrentRoom());
      if (classroom)
        SetCurrentRoom(classroom);
    }
  }
  if (DEBUG)
      std::cout << *this << GREEN << " [ATTEND CLASS]\t" << ENDC << std::endl;
}

void Student::ExitClass() {
  ClearCurrentRoom();
}

bool Student::InClass() const {
  return std::dynamic_pointer_cast<Classroom>(GetCurrentRoom()) != nullptr;
}

std::ostream& operator<<(std::ostream& s, const Student& stud) {
  s << BLUE << "[Student]" << ENDC << *(dynamic_cast<const Person*>(&stud));
  return (s);
}
