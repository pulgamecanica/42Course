//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"
#include "CourseFinishedForm.hpp"

CourseFinishedForm::CourseFinishedForm(): Form(FormType::CourseFinished), course_(nullptr) {
    ;
}

CourseFinishedForm::~CourseFinishedForm() {
    ;
}

void CourseFinishedForm::fill(Course *c) {
    course_ = c;
}

void CourseFinishedForm::execute() {
    if (course_ == nullptr) {
        std::cout << *this << RED << " can't be executed, form is not filled correctly." << ENDC << std::endl;
    } else if (isSigned()) {
        course_->finish();
        std::cout << *this << " has been executed! " << *course_ << GREEN << " [Finished]" << ENDC  << std::endl;
    } else {
        std::cout << *this << RED << " can't be executed, missing signature." << ENDC << std::endl;
    }
}


