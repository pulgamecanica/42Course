//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "CourseFinishedForm.hpp"

CourseFinishedForm::CourseFinishedForm(): Form(FormType::CourseFinished) {
    ;
}

CourseFinishedForm::~CourseFinishedForm() {
    // std::cout << "CourseFinishedForm" << " destroyed" << std::endl;
}

void CourseFinishedForm::execute() {
    signed_by_staff_ = true;
    std::cout << "Signed and executed by staff" << std::endl;
}


