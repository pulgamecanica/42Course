//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "SubscriptionToCourseForm.hpp"

SubscriptionToCourseForm::SubscriptionToCourseForm(): Form(FormType::SubscriptionToCourse) {
    ;
}

SubscriptionToCourseForm::~SubscriptionToCourseForm() {
    // std::cout << "SubscriptionToCourseForm" << " destroyed" << std::endl;
}

void SubscriptionToCourseForm::execute() {
    signed_by_staff_ = true;
    std::cout << "Signed and executed by staff" << std::endl;    
}
