//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"
#include "NeedCourseCreationForm.hpp"

NeedCourseCreationForm::NeedCourseCreationForm(const std::string & course_name, int number_of_class_to_graduate, int course_capacity):
    Form(FormType::NeedCourseCreation), course_name_(course_name), number_of_class_to_graduate_(number_of_class_to_graduate), course_capacity_(course_capacity) {
    new_course_ = nullptr;
}

NeedCourseCreationForm::~NeedCourseCreationForm() {
    // std::cout << "NeedCourseCreationForm" << " destroyed" << std::endl;
}

void NeedCourseCreationForm::execute() {
    if (new_course_ != nullptr) {
        if (DEBUG)
            std::cout << YELLOW << "[NeedCourseCreationForm] " << RED << "[EXECUTE FAIL]\t" << ENDC << *this << " has already been executed!" << std::endl; 
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



