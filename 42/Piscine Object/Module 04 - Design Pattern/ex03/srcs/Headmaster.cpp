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
#include "SubscriptionToCourseForm.hpp"

#include "ex03.inc"

Headmaster::Headmaster(const std::string& name)
  : Staff(name) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Headmaster]\t" << ENDC << name_ << std::endl;
}

Headmaster::~Headmaster() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Headmaster]\t" << ENDC << name_ << std::endl;
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
        std::cout << "Sorry " << sender << " but there are not courses right not for you to learn, wait until a professor opens subscriptions." << std::endl;
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

void Headmaster::SetSecretary(std::shared_ptr<Secretary> secretary) {
  secretary_ = secretary;
  if (DEBUG)
    std::cout << *this << GREEN << " [SET SECRETARY]\t" << ENDC << *secretary << std::endl;
}

bool Headmaster::HasSecretary() const {
  return !secretary_.expired();
}

std::ostream& operator<<(std::ostream& s, const Headmaster& hm) {
  s << BLUE << "[Headmaster]" << ENDC << *(dynamic_cast<const Staff*>(&hm));
  return (s);
}

