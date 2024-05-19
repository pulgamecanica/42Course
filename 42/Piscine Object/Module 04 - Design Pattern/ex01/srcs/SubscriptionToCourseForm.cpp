//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "SubscriptionToCourseForm.hpp"
#include "Student.hpp"
#include "Course.hpp"

#include "ex01.inc"

SubscriptionToCourseForm::SubscriptionToCourseForm()
  : Form(FormType::SubscriptionToCourse), course_(nullptr), student_(nullptr) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[SubscriptionToCourseForm]\t" << ENDC << std::endl;
}

SubscriptionToCourseForm::~SubscriptionToCourseForm() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[SubscriptionToCourseForm]\t" << ENDC << std::endl;
}

void SubscriptionToCourseForm::FillCourseAndStudent(std::shared_ptr<Course> course, std::shared_ptr<Student> student) {
  course_ = course;
  student_ = student;
}

void SubscriptionToCourseForm::Execute() {
  if (Errors() == 0) {
    course_->Subscribe(student_);
    if (DEBUG)
      std::cout << *this << GREEN << " [EXECUTE]\t" << ENDC << std::endl;
  } else {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " EXECUTE]" << ENDC << *this << " is not valid :(" << std::endl;
  }
}

unsigned SubscriptionToCourseForm::Errors() {
  unsigned errors = 0;

  if (!HasCourse())
    errors++;
  if (!HasStudent())
    errors++;
  return errors;
}

const std::vector<std::string> SubscriptionToCourseForm::ErrorsList() {
  std::vector<std::string> errors;

  if (!HasCourse())
    errors.push_back("Must have a valid course");
  if (!HasStudent())
    errors.push_back("Must have a valid student");
  return errors;
}

bool SubscriptionToCourseForm::HasCourse() {
  return course_ != nullptr;
}

bool SubscriptionToCourseForm::HasStudent() {
  return student_ != nullptr;
}
std::ostream& operator<<(std::ostream& s, const SubscriptionToCourseForm& sc) {
    s << BLUE << "[SubscriptionToCourseForm]" << ENDC << *(dynamic_cast<const Form*>(&sc));
  return (s);
}

