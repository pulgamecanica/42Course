//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"
#include "SubscriptionToCourseForm.hpp"

SubscriptionToCourseForm::SubscriptionToCourseForm():
    Form(FormType::SubscriptionToCourse), student_(nullptr), course_(nullptr) {
    ;
}

SubscriptionToCourseForm::~SubscriptionToCourseForm() {
    ;
}

void SubscriptionToCourseForm::fill(Course * course, Student * student) {
    course_ = course;
    student_ = student;
}

void SubscriptionToCourseForm::execute() {
    if (course_ == nullptr || student_ == nullptr) {
        std::cout << *this << RED << " can't be executed, form is not filled correctly." << ENDC << std::endl;
    } else if (isSigned()) {
        course_->subscribe(student_);
        std::cout << *this << " has been executed! " << *student_ << GREEN << " [Requested Subscription]" << ENDC << " to " << *course_ << std::endl;
    } else {
        std::cout << *this << RED << " can't be executed, missing signature." << ENDC << std::endl;
    } 
}
