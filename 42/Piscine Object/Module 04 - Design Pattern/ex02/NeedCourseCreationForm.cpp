//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "NeedCourseCreationForm.hpp"

NeedCourseCreationForm::NeedCourseCreationForm(): Form(FormType::NeedCourseCreation) {
    ;
}

NeedCourseCreationForm::~NeedCourseCreationForm() {
    // std::cout << "NeedCourseCreationForm" << " destroyed" << std::endl;
}

void NeedCourseCreationForm::execute() {
    signed_by_staff_ = true;
    std::cout << "Signed and executed by staff" << std::endl;    
}


