//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"
#include "SubscriptionToCourseForm.hpp"

SubscriptionToCourseForm::SubscriptionToCourseForm(Course * course, Student * student):
    Form(FormType::SubscriptionToCourse), student_(student), course_(course) {
    ;
}

SubscriptionToCourseForm::~SubscriptionToCourseForm() {
    // std::cout << "SubscriptionToCourseForm" << " destroyed" << std::endl;
}

void SubscriptionToCourseForm::execute() {
    if (course_ == nullptr || student_ == nullptr) {
        if (DEBUG)
            std::cout << "[CourseFinishedForm] " << RED << "EXECUTE FAIL\t" << ENDC << " form is corrupted" << std::endl;
        return ;
    }
    if (isSigned()) {
        course_->subscribe(student_);
        std::cout << *this << " has been executed! " << *student_ << GREEN << " [Requested Subscription]" << ENDC << " to " << *course_ << std::endl;
    } else {
        std::cout << *this << RED << " can't be executed, missing signature." << ENDC << std::endl;
    } 
}
