//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "NeedMoreClassRoomForm.hpp"
#include "Classroom.hpp"
#include "RoomList.hpp"

#include "ex04.inc"

NeedMoreClassRoomForm::NeedMoreClassRoomForm()
  : Form(FormType::NeedMoreClassRoom), class_room_(nullptr) {
  if (DEBUG)
    std::cout << GREEN << "[NEW]" << YELLOW << "[NeedMoreClassRoomForm]\t" << ENDC << std::endl;
}

NeedMoreClassRoomForm::~NeedMoreClassRoomForm() {
  if (DEBUG)
    std::cout << RED << "[DESTROY]" << YELLOW << "[NeedMoreClassRoomForm]\t" << ENDC << std::endl;
}

std::shared_ptr<Classroom> NeedMoreClassRoomForm::GetClassroom() {
  return class_room_;
}

void NeedMoreClassRoomForm::Execute() {
  if (IsSigned() && !class_room_) {
    class_room_ = std::shared_ptr<Classroom>(new Classroom());
    RoomList::GetInstance()->add(class_room_); // Add classroom so other teachers can find it later
    if (DEBUG)
      std::cout << *this << GREEN << " [EXECUTE]\t" << ENDC << "Classroom created" << std::endl;
  } else {
    std::cout << RED << "[" << YELLOW << "Warning" << RED << " EXECUTE]" << ENDC << *this << " cannot be executed [ Valid: " << std::boolalpha << IsValid() << std::noboolalpha << ", Signed: " << std::boolalpha << IsSigned() << std::noboolalpha << " ]" << std::endl;
  }
}

unsigned NeedMoreClassRoomForm::Errors() {
  return 0;
}

const std::vector<std::string> NeedMoreClassRoomForm::ErrorsList() {
  return std::vector<std::string>();
}

std::ostream& operator<<(std::ostream& s, const NeedMoreClassRoomForm& nmc) {
    s << BLUE << "[NeedMoreClassRoomForm]" << ENDC << *(dynamic_cast<const Form*>(&nmc));
  return (s);
}

