//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"
#include "NeedMoreClassRoomForm.hpp"

NeedMoreClassRoomForm::NeedMoreClassRoomForm(): Form(FormType::NeedMoreClassRoom) {
    class_room_ = nullptr;
}

NeedMoreClassRoomForm::~NeedMoreClassRoomForm() {
    // std::cout << "NeedMoreClassRoomForm" << " destroyed" << std::endl;
}

ClassRoom *NeedMoreClassRoomForm::getClassRoom() {
    return class_room_;
}

void NeedMoreClassRoomForm::execute() {
    if (class_room_ != nullptr) {
        std::cout << *this << RED << " can't be executed, can only be executed once." << ENDC << std::endl;
    } else if (isSigned()) {
        class_room_ = new ClassRoom();
        std::cout << *this << " has been executed! " << *class_room_ << GREEN << " [Created]" << ENDC  << std::endl;
    } else {
        std::cout << *this << RED << " can't be executed, missing signature." << ENDC << std::endl;
    }
}
