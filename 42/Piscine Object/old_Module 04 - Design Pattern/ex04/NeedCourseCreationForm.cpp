//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"
#include "NeedCourseCreationForm.hpp"

NeedCourseCreationForm::NeedCourseCreationForm():
    Form(FormType::NeedCourseCreation), course_name_(""), number_of_class_to_graduate_(-1), course_capacity_(-1), new_course_(nullptr) {
}

NeedCourseCreationForm::~NeedCourseCreationForm() {
    ;
}

void NeedCourseCreationForm::fill(const std::string & course_name, int number_of_class_to_graduate, int course_capacity) {
    course_name_ = course_name;
    number_of_class_to_graduate_ = number_of_class_to_graduate;
    course_capacity_ = course_capacity;
}

void NeedCourseCreationForm::execute() {
    if (new_course_ != nullptr) {
        std::cout << *this << RED << " can't be executed, can only be executed once." << ENDC << std::endl;
    } else if (course_name_.empty() || number_of_class_to_graduate_ < 0 || course_capacity_ < 0) {
        std::cout << *this << RED << " can't be executed, form is not filled correctly." << ENDC << std::endl;
    } else if (isSigned()) {
        new_course_ = new Course(course_name_, number_of_class_to_graduate_, course_capacity_);
        std::cout << *this << " has been executed! " << *new_course_ << GREEN << " [Created]" << ENDC  << std::endl;
    } else {
        std::cout << *this << RED << " can't be executed, missing signature." << ENDC << std::endl;
    }
}

Course *NeedCourseCreationForm::getCourse() {
    return new_course_;
}



