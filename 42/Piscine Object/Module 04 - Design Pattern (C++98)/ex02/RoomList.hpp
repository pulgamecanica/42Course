//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ROOMLIST_HPP__
# define __ROOMLIST_HPP__

#include <iostream>

#include "Singleton.hpp"
#include "Room.hpp"

class RoomList: public Singleton<Room, RoomList> {
    public:
        RoomList();
        ~RoomList();
    private:
};
std::ostream& operator<<(std::ostream&, const RoomList&);
#endif
