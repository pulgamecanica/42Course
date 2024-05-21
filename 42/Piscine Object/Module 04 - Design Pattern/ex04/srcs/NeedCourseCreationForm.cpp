//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "NeedCourseCreationForm.hpp"
#include "Course.hpp"
#include "Professor.hpp"

#include "ex04.inc"

NeedCourseCreationForm::NeedCourseCreationForm()
  : Form(FormType::NeedCourseCreation), course_(nullptr), capacity_(0), total_lessons_(0) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[NeedCourseCreationForm]\t" << ENDC << std::endl;
}

NeedCourseCreationForm::~NeedCourseCreationForm() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[NeedCourseCreationForm]\t" << ENDC << std::endl;
}

std::shared_ptr<Course> NeedCourseCreationForm::GetCourse() {
  return course_;
}

void NeedCourseCreationForm::FillCourse(std::string name, unsigned total_lessons, unsigned capacity, std::shared_ptr<Professor> professor) {
  name_ = name;
  capacity_ = capacity;
  total_lessons_ = total_lessons;
  professor_ = professor;
}

void NeedCourseCreationForm::Execute() {
  if (IsSigned() && !Errors() && !course_) {
    course_ = std::shared_ptr<Course>(new Course(name_, total_lessons_, capacity_));
    course_->Assign(professor_.lock());
    if (DEBUG)
      std::cout << *this << GREEN << " [EXECUTE]\t" << ENDC << "Created Course: " << name_ << ENDC << std::endl;
  } else {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " EXECUTE]" << ENDC << *this << " cannot be executed [ Valid: " << std::boolalpha << IsValid() << std::noboolalpha << ", Signed: " << std::boolalpha << IsSigned() << std::noboolalpha << " ]" << std::endl;
  }
}

unsigned NeedCourseCreationForm::Errors() {
  unsigned errors = 0;
  if (!HasName())
    errors++;
  if (!ValidCapacity())
    errors++;
  if (!ValidLessons())
    errors++;
  if (!HasProfessor())
    errors++;
  return errors;
}

const std::vector<std::string> NeedCourseCreationForm::ErrorsList() {
  std::vector<std::string> errors;
  if (!HasName())
    errors.push_back("Course must have a name");
  if (!ValidCapacity())
    errors.push_back("The capacity must be greater than zero");
  if (!ValidLessons())
    errors.push_back("The total lessons must be greater than zero");
  if (!HasProfessor())
    errors.push_back("Must have a valid professor");
  return errors;
}

bool NeedCourseCreationForm::HasName() {
  return !name_.empty();
}

bool NeedCourseCreationForm::ValidCapacity() {
  return capacity_ > 0;
}

bool NeedCourseCreationForm::ValidLessons() {
  return total_lessons_ > 0;
}

bool NeedCourseCreationForm::HasProfessor() {
  return !professor_.expired() && professor_.lock();
}

std::ostream& operator<<(std::ostream& s, const NeedCourseCreationForm& ncc) {
    s << BLUE << "[NeedCourseCreationForm]" << ENDC << *(dynamic_cast<const Form*>(&ncc));
    return (s);
}
