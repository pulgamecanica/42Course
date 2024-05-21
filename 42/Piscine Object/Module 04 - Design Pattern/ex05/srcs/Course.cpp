//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Course.hpp"
#include "Professor.hpp"
#include "Student.hpp"

#include <algorithm>

#include "ex04.inc"

Course::Course(const std::string & name, unsigned lessons_to_graduate, unsigned capacity)
  : name_(name), lessons_to_graduate_(lessons_to_graduate), capacity_(capacity), finished_(false) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Course]\t" << ENDC << name_ << std::endl;
}

Course::~Course() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Course]\t" << ENDC << name_ << std::endl;
}

std::ostream& Course::LoadCourseInfo(std::ostream& s) const {
  s << BLUE << "[Course] " << name_ << ENDC;
  if (!responsible_.expired())
      s << " by " << BLUE << responsible_.lock()->GetName() << ENDC << ", ";
  else
      s << YELLOW << " no responsible, " << ENDC;
  s << students_.size() << "/" << capacity_ << " students, lessons left: " << lessons_to_graduate_;
  return s;
}


void Course::Subscribe(std::shared_ptr<Student> student) {
  if (finished_)
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " SUBSCRIBE]" << ENDC << *this << " Course has finished, cannot subscribe ." << std::endl;
  else if (HasStudent(student)) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " SUBSCRIBE]" << ENDC << *this << " Cannot subscribe same student multiple times." << std::endl;
  } else if ((unsigned)students_.size() < capacity_) {
    students_.push_back(student);
    student->AddCourse(shared_from_this());
    if (DEBUG)
      std::cout << *this << GREEN << " [SUBSCRIBE]\t" << ENDC << *student << std::endl;
  } else {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " SUBSCRIBE]" << ENDC << *this << " Course capacity reached." << std::endl;
  }
}

bool Course::Graduate(std::shared_ptr<Student> student) {
  auto it = std::remove_if(students_.begin(), students_.end(),
             [&student](const std::weak_ptr<Student>& s) {
               return s.lock() == student;
             });
  if (it == students_.end()) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " GRADUATE]" << ENDC << *this << " cannot graduate student which is not subscribed." << std::endl;
  } else if (lessons_to_graduate_ > 0) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " GRADUATE]" << ENDC << *this << " course has not finished, cannot graduate, attend your classes!" << std::endl;
  } else {
    students_.erase(it, students_.end());
    student->Graduate(shared_from_this());
    if (DEBUG)
      std::cout << *this << GREEN << " [GRADUATE]\t" << ENDC << *student << std::endl;
    return true;
  }
  return false;
}

void Course::EndCourse() {
  if (!finished_) {
    std::vector<std::weak_ptr<Student> > tmp = students_;
    for (auto student : tmp)
      if (!student.expired() && student.lock())
        Graduate(student.lock());
    finished_ = true;
    lessons_to_graduate_ = 0;
  }
}

void Course::GiveClass() {
  if (lessons_to_graduate_ == 0) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " GIVE CLASS]" << ENDC << *this << " Has no more classes to be given." << std::endl;
  } else {
    lessons_to_graduate_--;
    if (DEBUG)
      std::cout << *this << GREEN << " [GIVE CLASS]\t" << ENDC << std::endl;
  }
}

// Calling assign with nullptr is undefined
void Course::Assign(std::shared_ptr<Professor> professor) {
  auto current_professor = responsible_.lock();
  if (HasProfessor(professor)) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " ASSIGN]" << ENDC << *this << " Assigning same professor multiple times." << std::endl;
    return;
  }
  if (current_professor) {
    current_professor->RemoveCourse();
  }
  responsible_ = professor;
  professor->RemoveCourse();
  professor->SetCourse(shared_from_this());
  if (DEBUG)
    std::cout << *this << GREEN << " [ASSIGN]\t" << ENDC << *professor << std::endl;
}

std::shared_ptr<Professor> Course::GetResponsible() const {
  return responsible_.expired() ? nullptr : responsible_.lock();
}

unsigned Course::LessonsLeft() const {
  return lessons_to_graduate_;
}

bool Course::IsFinished() const {
  return finished_;
}

bool Course::IsFull() const {
  return students_.size() >= capacity_;
}

const std::string& Course::GetName() const {
  return name_;
}

bool Course::HasStudent(std::shared_ptr<Student> student) const {
  return std::any_of(students_.begin(), students_.end(),
          [student](const std::weak_ptr<Student>& weak_ptr) {
              return !weak_ptr.expired() && weak_ptr.lock() == student;
          });
}

bool Course::HasProfessor(std::shared_ptr<Professor> professor) const {
    return !responsible_.expired() && professor == responsible_.lock();
}

std::ostream& operator<<(std::ostream& s, const Course& c) {
    return c.LoadCourseInfo(s);
}
