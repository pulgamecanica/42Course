//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Secretary.hpp"
#include "SecretarialOffice.hpp"
#include "CourseFinishedForm.hpp"
#include "NeedMoreClassRoomForm.hpp"
#include "NeedCourseCreationForm.hpp"
#include "SubscriptionToCourseForm.hpp"
#include "RoomList.hpp"

#include "ex04.inc"

Secretary::Secretary(const std::string& name)
  : Staff(name) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[Secretary]\t" << ENDC << name_ << std::endl;
}

Secretary::~Secretary() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[Secretary]\t" << ENDC << name_ << std::endl;
}

void Secretary::EnterOffice() {
  auto office = std::dynamic_pointer_cast<SecretarialOffice>(RoomList::GetInstance()->getFirst<SecretarialOffice>());
  if (office) {
    SetCurrentRoom(office);
    if (DEBUG)
      std::cout << *this << GREEN << " [ENTER OFFICE]\t" << ENDC << std::endl;
  } else {
    std::cout << "Warning the school secretary couldn't find the SecretarialOffice" << std::endl;
  }
}

std::shared_ptr<Form> Secretary::CreateForm(FormType type) {
  // Factory to generate forms
  switch (type) {
    case FormType::CourseFinished:
      return std::shared_ptr<Form>(new CourseFinishedForm());
    case FormType::NeedCourseCreation:
      return std::shared_ptr<Form>(new NeedCourseCreationForm());
    case FormType::SubscriptionToCourse:
      return std::shared_ptr<Form>(new SubscriptionToCourseForm());
    case FormType::NeedMoreClassRoom:
      return std::shared_ptr<Form>(new NeedMoreClassRoomForm());
    default: return nullptr;
  }
}

void Secretary::ArchiveForm(std::shared_ptr<Form>& form) {
  /**
   * (2 other ways to get the office, choose depending on the implementation)
   * Example:
   * SecretarialOffice & so = dynamic_cast<SecretarialOffice&>((*room));
   * SecretarialOffice * so = dynamic_cast<SecretarialOffice*>(&(*room));
   **/
  auto so = std::dynamic_pointer_cast<SecretarialOffice>(GetCurrentRoom());
  if (so) {
    so->ArchiveForm(form);
    if (DEBUG)
      std::cout << *this << GREEN << " [ARCHIVE FORM]\t" << ENDC << *form << std::endl;
  } else {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " ArchiveForm]" << ENDC << *this << " Cannot achive form while outise the office." << std::endl;
  }
}

std::shared_ptr<Course> Secretary::GetCourseNotFinished() {
  auto so = std::dynamic_pointer_cast<SecretarialOffice>(GetCurrentRoom());
  if (so)
    return so->GetCourseNotFinishedAndAvailableFromForms();
  else
    return nullptr;
}



std::ostream& operator<<(std::ostream& s, const Secretary& secretary) {
  s << BLUE << "[Secretary]" << ENDC << *(dynamic_cast<const Staff*>(&secretary));
  return (s);
}

