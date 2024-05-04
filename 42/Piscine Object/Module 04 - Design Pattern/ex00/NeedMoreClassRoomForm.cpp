//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "NeedMoreClassRoomForm.hpp"

NeedMoreClassRoomForm::NeedMoreClassRoomForm(): Form(FormType::NeedMoreClassRoom) {
    ;
}

NeedMoreClassRoomForm::~NeedMoreClassRoomForm() {
    // std::cout << "NeedMoreClassRoomForm" << " destroyed" << std::endl;
}

void NeedMoreClassRoomForm::execute() {
    signed_by_staff_ = true;
    std::cout << "Signed and executed by staff" << std::endl;    
}
