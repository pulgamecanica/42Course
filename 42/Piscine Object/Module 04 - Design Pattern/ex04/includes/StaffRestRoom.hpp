//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STAFFRESTROOM_HPP__
# define __STAFFRESTROOM_HPP__

#include "Room.hpp"

#include <iostream>

class StaffRestRoom: public Room {
  public:
  StaffRestRoom();
  ~StaffRestRoom();
  StaffRestRoom(const StaffRestRoom&) = default;
  StaffRestRoom& operator= (const StaffRestRoom&) = default;
  bool CanEnter(std::shared_ptr<Person> person);
  
  private:
};
std::ostream& operator<<(std::ostream&, const StaffRestRoom&);
#endif
