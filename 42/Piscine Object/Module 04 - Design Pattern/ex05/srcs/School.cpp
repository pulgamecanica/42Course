//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.hpp"
#include "StaffList.hpp"
#include "CourseList.hpp"
#include "StudentList.hpp"
#include "IObserver.hpp"
#include "Professor.hpp"
#include "Student.hpp"

#include <sstream>
#include <memory>

#include "ex04.inc"

School::School() : log_(std::cout),
  header_(std::string(BLUE) + std::string("[School]") + std::string(ENDC)), day_(1) {
  log_.setHeader(&header_);
  headmaster_ = std::shared_ptr<Headmaster>(new Headmaster("Principal Skinner"));
  secretary_ =  std::shared_ptr<Secretary>(new Secretary("Waylon Smithers"));
  headmaster_->SetSecretary(secretary_);
  secretary_->EnterOffice();
  headmaster_->EnterOffice();
  StaffList::GetInstance()->add(secretary_);
  Log("School created");
}

School::~School() {
  Log("School destroyed");
}

void School::Log(const std::string & msg) {
  log_.write(msg);
}

void School::RunDayRoutine() {
  if (day_ % 7 == 0 || day_ % 6 == 0) {
    std::cout << *this << YELLOW << " WISHES YOU A GREAT WEEKEND" << ENDC << std::endl;
  } else {
    std::cout << YELLOW << "LAUNCHING DAY " << BLUE << day_ << YELLOW << " HAVE A GREAT STUDY" << ENDC << std::endl;
    LaunchClasses();
    RequestRingBell();
    LaunchClasses();
    headmaster_->FoodTruckArrived(); // Request Lunch! YAY :D
    LaunchClasses();
    RequestRingBell();
    LaunchClasses();
  }
  day_++;
}

void School::LaunchClasses() {
  for (auto& prof : professors_)
    if (!prof.expired() && prof.lock())
      headmaster_->MakeProfessorTeachClass(prof.lock());
  for (auto& stud : students_)
    if (!stud.expired() && stud.lock())
      headmaster_->MakeStudentAttendClass(stud.lock());
}

void School::RequestRingBell() {
  headmaster_->RingBell();
}

void School::RecruteProfessor(std::shared_ptr<Professor> prof) {
  professors_.push_back(prof);
  prof->SetMediator(headmaster_);
  headmaster_->SetObserver(prof);
  StaffList::GetInstance()->add(prof); // Optional, cool right?
  Log("Recruted professor");
} 

void School::RecruteStudent(std::shared_ptr<Student> stud) {
  students_.push_back(stud);
  stud->SetMediator(headmaster_);
  headmaster_->SetObserver(stud);
  StudentList::GetInstance()->add(stud); // Optional
  Log("Recruted Student");
}

std::shared_ptr<Course> School::GetCourse(std::string name) {
  for (auto& course : CourseList::GetInstance()->getItems()) {
    if (course->GetName() == name)
      return course;
  }
  return nullptr;
}

std::vector<std::weak_ptr<Student>> School::GetStudents() {
  return students_;
}

std::vector<std::weak_ptr<Professor>> School::GetProfessors() {
  return professors_;
}

const std::shared_ptr<Headmaster>& School::GetHeadmaster() const {
  return headmaster_;
}

const std::shared_ptr<Secretary>& School::GetSecretary() const {
  return secretary_;
}

void School::GraduationCeremony() {
  for (auto& course : CourseList::GetInstance()->getItems())
    if (course->LessonsLeft() == 0)
        course->EndCourse();
}

std::ostream& operator<<(std::ostream& s, const School& school) {
  s << *school.GetHeadmaster() << "'s School" << ENDC;
  return (s);
}

