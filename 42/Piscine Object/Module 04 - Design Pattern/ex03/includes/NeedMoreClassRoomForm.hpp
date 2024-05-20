//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __NEEDMORECLASSROOMFORM_HPP__
# define __NEEDMORECLASSROOMFORM_HPP__

#include "Form.hpp"

#include <iostream>
#include <memory>

class Classroom;

class NeedMoreClassRoomForm: public Form {
  public:
  NeedMoreClassRoomForm();
  ~NeedMoreClassRoomForm();
  NeedMoreClassRoomForm(const NeedMoreClassRoomForm&) = default;
  NeedMoreClassRoomForm& operator= (const NeedMoreClassRoomForm&) = default;
  std::shared_ptr<Classroom> GetClassroom();
  void Execute();
  unsigned Errors();
  const std::vector<std::string> ErrorsList();

  private:
  std::shared_ptr<Classroom> class_room_;
};
std::ostream& operator<<(std::ostream&, const NeedMoreClassRoomForm&);
#endif
