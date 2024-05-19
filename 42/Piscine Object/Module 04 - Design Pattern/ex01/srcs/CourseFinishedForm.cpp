//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "CourseFinishedForm.hpp"
#include "Course.hpp"

#include <sstream>

#include "ex01.inc"

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
  if (course && !course_.expired()) {
    course_ = course;
  } // else there is no effect, can only fill if the form has no valid course
}


void CourseFinishedForm::Execute() {
  ; // Set course as finished!
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

