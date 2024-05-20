//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "SecretarialOffice.hpp"
#include "Secretary.hpp"
#include "Form.hpp"
#include "Course.hpp"
#include "NeedCourseCreationForm.hpp"

#include <memory>

#include "ex03.inc"

SecretarialOffice::SecretarialOffice() {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[SecretarialOffice]\t" << ENDC << id_ << std::endl;
}

SecretarialOffice::~SecretarialOffice() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[SecretarialOffice]\t" << ENDC << id_ << std::endl;
}

void SecretarialOffice::ArchiveForm(std::shared_ptr<Form>& form) {
  archived_forms_.push_back(form);
  if (DEBUG)
    std::cout << *this << GREEN << " [ARCHIVE FORM]\t" << ENDC << *form << std::endl;
}

bool SecretarialOffice::CanEnter(std::shared_ptr<Person> person) {
  return (
    std::dynamic_pointer_cast<Secretary>(person) != nullptr);
}

std::shared_ptr<Course> SecretarialOffice::GetCourseNotFinishedFromForms() {
  for (auto form : archived_forms_) {
    auto course_form = std::dynamic_pointer_cast<NeedCourseCreationForm>(form);
    if (course_form) {
      if (!course_form->GetCourse()->IsFinished())
        return course_form->GetCourse();
    }
  }
  return nullptr;
}


std::ostream& operator<<(std::ostream& s, const SecretarialOffice& so) {
  s << BLUE << "[SecretarialOffice]" << ENDC << *(dynamic_cast<const Room*>(&so));
  return (s);
}

