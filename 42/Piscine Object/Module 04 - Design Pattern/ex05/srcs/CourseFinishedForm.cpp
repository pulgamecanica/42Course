//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "CourseFinishedForm.hpp"
#include "Course.hpp"

#include <sstream>

#include "ex04.inc"

CourseFinishedForm::CourseFinishedForm()
  : Form(FormType::CourseFinished) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[CourseFinishedForm]\t" << ENDC << std::endl;
}

CourseFinishedForm::~CourseFinishedForm() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[CourseFinishedForm]\t" << ENDC << std::endl;
}

void CourseFinishedForm::FillCourse(std::shared_ptr<Course> course) {
  course_ = course;
  if (DEBUG)
    std::cout << *this << GREEN << " [FILL COURSE]\t" << ENDC << *course << std::endl;
}

void CourseFinishedForm::Execute() {
  if (IsValid() && IsSigned()) {
    course_.lock()->EndCourse();
    if (DEBUG)
      std::cout << *this << GREEN << " [EXECUTE]\t" << ENDC << "Course Finished!" << std::endl;
  } else {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " EXECUTE]" << ENDC << *this << " cannot be executed [ Valid: " << std::boolalpha << IsValid() << std::noboolalpha << ", Signed: " << std::boolalpha << false << std::noboolalpha << " ]" << std::endl;
  }
};

unsigned CourseFinishedForm::Errors() {
  unsigned errors = 0;

  if (!ValidCourse())
    errors++;
  if (!CourseCanFinish())
    errors++;
  return errors;
}

const std::vector<std::string> CourseFinishedForm::ErrorsList() {
  std::vector<std::string> errors;
  if (!ValidCourse()) {
    errors.push_back("Invalid Course");
    errors.push_back("Course has not finished");
  } else if (!CourseCanFinish()) {
    std::stringstream ss;
    ss << *course_.lock() << " has not finished " << course_.lock()->LessonsLeft() << " lessons left";
    errors.push_back(ss.str());
  }
  return errors;
};

bool CourseFinishedForm::ValidCourse() const {
  return !course_.expired();
}

bool CourseFinishedForm::CourseCanFinish() const {
  if (ValidCourse())
    return course_.lock()->LessonsLeft() == 0;
  return false;
}

std::ostream& operator<<(std::ostream& s, const CourseFinishedForm& cff) {
  s << BLUE << "[CourseFinishedForm]" << ENDC << *(dynamic_cast<const Form*>(&cff));
  return (s);
}

