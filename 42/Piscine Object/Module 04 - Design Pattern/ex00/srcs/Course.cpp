//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Course.hpp"
#include "Professor.hpp"
#include "Student.hpp"

#include <algorithm>

#include "ex00.inc"

Course::Course(const std::string & name, int total_lessons, int capacity)
  : name_(name), total_lessons_(total_lessons), capacity_(capacity) {
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
  s << students_.size() << "/" << capacity_ << " students, total lessons: " << total_lessons_;
  return s;
}


void Course::Subscribe(std::shared_ptr<Student> student) {
  if (HasStudent(student)) {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " SUBSCRIBE]" << ENDC << *this << " Cannot subscribe same student multiple times." << std::endl;
    return ;
  }
  if ((int)students_.size() < capacity_) {
    students_.push_back(student);
    student->AddCourse(shared_from_this());
    if (DEBUG)
      std::cout << GREEN << "[SUBSCRIBE]" << *this << "\t" << ENDC << *student << std::endl;
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
    return false;
  } else {
    // if student can graduate (has attended X lessons)
    students_.erase(it, students_.end());
    student->Graduate(shared_from_this());
    if (DEBUG)
      std::cout << GREEN << "[GRADUATE]" << ENDC << *this << "\t" << *student << std::endl;
    return true;
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
  professor->SetCourse(shared_from_this());
  if (DEBUG)
    std::cout << GREEN << "[ASSIGN]" << ENDC << *this << "\t" << *professor << std::endl;
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
