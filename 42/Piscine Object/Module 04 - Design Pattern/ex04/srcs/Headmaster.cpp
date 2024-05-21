//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Headmaster.hpp"
#include "Form.hpp"
#include "Student.hpp"
#include "Professor.hpp"
#include "Secretary.hpp"
#include "Classroom.hpp"
#include "CourseFinishedForm.hpp"
#include "NeedCourseCreationForm.hpp"
#include "NeedMoreClassRoomForm.hpp"
#include "HeadmasterOffice.hpp"
#include "SubscriptionToCourseForm.hpp"
#include "RoomList.hpp"
#include "StaffList.hpp"
#include "StudentList.hpp"

#include "ex04.inc"

Headmaster::Headmaster(const std::string& name)
  : Staff(name), bell_() {
  SetSecretary();
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Headmaster]\t" << ENDC << name_ << std::endl;
}

Headmaster::~Headmaster() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Headmaster]\t" << ENDC << name_ << std::endl;
}

void Headmaster::EnterOffice() {
  auto office = std::dynamic_pointer_cast<HeadmasterOffice>(RoomList::GetInstance()->getFirst<HeadmasterOffice>());
  if (office) {
    SetCurrentRoom(office);
    if (DEBUG)
      std::cout << *this << GREEN << " [ENTER OFFICE]\t" << ENDC << std::endl;
    if (!HasSecretary())
      SetSecretary();
  } else {
    std::cout << "Warning the Headmaster couldn't find his office" << std::endl;
  }
}

void Headmaster::ReceiveForm(std::shared_ptr<Form> form) const {
  if (!HasSecretary()) {
    std::cout << "Warning headmaster can't sign forms without a secretary to archive them!" << std::endl;
    return ;
  }
  Sign(form);
  if (form->Errors() == 0) {
    form->Execute();
    secretary_.lock()->ArchiveForm(form);
  }
  if (DEBUG)
    std::cout << *this << GREEN << " [RECEIVE FORM]\t" << ENDC << *form << std::endl;
}

void Headmaster::Notify(std::shared_ptr<Person> sender, enum Notification notification) const {
  if (!HasSecretary()) {
    std::cout << "Warning headmaster can't issue forms without a secretary!" << std::endl;
    return ;
  }
  std::shared_ptr<Secretary> s = secretary_.lock();
  switch (notification) {
    case ProfessorRequestGraduation : {
      auto cf = std::dynamic_pointer_cast<CourseFinishedForm>(s->CreateForm(FormType::CourseFinished)); /* I need a form for graduation -> here take this form*/
      cf->FillCourse((std::dynamic_pointer_cast<Professor>(sender))->GetCurrentCourse()); /* Can you full this form */
      ReceiveForm(cf); /* Sign and execute */
    } return ;
    case StudentRequestToJoinCourse : {
      auto stc = std::dynamic_pointer_cast<SubscriptionToCourseForm>(s->CreateForm(FormType::SubscriptionToCourse));
      std::shared_ptr<Course> course = s->GetCourseNotFinished();
      if (course) {
        stc->FillCourseAndStudent(course, std::dynamic_pointer_cast<Student>(sender));
        ReceiveForm(stc);
      } else
        std::cout << "Sorry " << *sender << " but there are not courses right not for you to learn, wait until a professor opens subscriptions." << std::endl;
    } return ;
    case ProfessorRequestNewCourse : {
      auto prof = std::dynamic_pointer_cast<Professor>(sender);
      auto ncc = std::dynamic_pointer_cast<NeedCourseCreationForm>(s->CreateForm(FormType::NeedCourseCreation)); /* I need a course to teach (default: Professors's name course) */
      ncc->FillCourse(prof->GetName() + "'s Course", 9, 9, prof); /* Can you full this form */
      ReceiveForm(ncc); /* Sign and execute */
      prof->Teach();
    } return ;
    case ProfessorRequestNewClassroom : {
      auto nmc = std::dynamic_pointer_cast<NeedMoreClassRoomForm>(s->CreateForm(FormType::NeedMoreClassRoom)); /* I need a course to teach (default: Professors's name course) */
      auto prof = std::dynamic_pointer_cast<Professor>(sender);
      ReceiveForm(nmc);
      prof->SetCurrentRoom(nmc->GetClassroom());
      nmc->GetClassroom()->SetCourse(prof->GetCurrentCourse());
      prof->Teach();
    } return ;
  }
}

void Headmaster::MakeStudentAttendClass(std::shared_ptr<Student> student) {
  student->AttendClass();
  if (DEBUG)
    std::cout << *this << GREEN << " [MAKE STUDENT ATTEND CLASS]\t" << ENDC << *student << std::endl;
}

void Headmaster::MakeProfessorTeachClass(std::shared_ptr<Professor> professor) {
  professor->Teach();
  if (DEBUG)
    std::cout << *this << GREEN << " [MAKE PROFESSOR TEACH]\t" << ENDC << *professor << std::endl;
}

void Headmaster::SetSecretary() {
  auto secretary = std::dynamic_pointer_cast<Secretary>(StaffList::GetInstance()->getFirst<Secretary>());
  if (secretary) {
    secretary_ = secretary;
    if (DEBUG)
      std::cout << *this << GREEN << " [SET SECRETARY]\t" << ENDC << *secretary << std::endl;
  } else {
    std::cout << "Warning the Headmaster couldn't find a secretary" << std::endl;
  }
}

bool Headmaster::HasSecretary() const {
  return !secretary_.expired();
}

void Headmaster::RingBell() {
  std::cout << BLUE << "    ______________________________ " << std::endl;
  std::cout << BLUE << " / \\                             \\. " << std::endl;
  std::cout << BLUE << "|   |                            |. " << std::endl;
  std::cout << BLUE << " \\_ |         " << GREEN << "BELL RINGS" << BLUE << "         |. " << std::endl;
  std::cout << BLUE << "    |   _________________________|___ " << std::endl;
  std::cout << BLUE << "    |  /                            /. " << std::endl;
  std::cout << BLUE << "    \\_/____________________________/. " << std::endl;
  bell_.Notify();
  if (DEBUG)
    std::cout << *this << GREEN << " [RING BELL]\t" << ENDC << std::endl;
}

// All teachers and students should subscribe to the bell
void Headmaster::SetObservers() {
  for(auto& staff : StaffList::GetInstance()->getItems())
    if (std::dynamic_pointer_cast<Professor>(staff))
      bell_.Attach(std::dynamic_pointer_cast<Professor>(staff));
  for(auto& stud : StudentList::GetInstance()->getItems())
    bell_.Attach(stud);
}

std::ostream& operator<<(std::ostream& s, const Headmaster& hm) {
  s << BLUE << "[Headmaster]" << ENDC << *(dynamic_cast<const Staff*>(&hm));
  return (s);
}

