//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"
#include "CourseFinishedForm.hpp"

CourseFinishedForm::CourseFinishedForm(Course * c):
    Form(FormType::CourseFinished), course_(c) {
    ;
}

CourseFinishedForm::~CourseFinishedForm() {
    // std::cout << "CourseFinishedForm" << " destroyed" << std::endl;
}

void CourseFinishedForm::execute() {
    if (course_ == nullptr) {
        if (DEBUG)
            std::cout << "[CourseFinishedForm] " << RED << "EXECUTE FAIL\t" << ENDC << " form is corrupted" << std::endl;
        return ;
    }
    if (isSigned()) {
        course_->finish();
        std::cout << *this << " has been executed! " << *course_ << GREEN << " [Finished]" << ENDC  << std::endl;
    } else {
        std::cout << *this << RED << " can't be executed, missing signature." << ENDC << std::endl;
    }
}


