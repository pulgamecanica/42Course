//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ROOMLIST_HPP__
# define __ROOMLIST_HPP__

#include "Singleton.hpp"
#include "Room.hpp"

#include <iostream>

class RoomList: public Singleton<Room> {
  public:
  ~RoomList();
  static RoomList *GetInstance();

  private:
  RoomList();
  static RoomList * instance_;
};
std::ostream& operator<<(std::ostream&, const RoomList&);
#endif
