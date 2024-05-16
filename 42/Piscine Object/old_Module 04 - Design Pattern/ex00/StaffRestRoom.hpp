//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STAFFRESTROOM_HPP__
# define __STAFFRESTROOM_HPP__

#include <iostream>

#include "Room.hpp"

class StaffRestRoom: public Room {
    public:
        StaffRestRoom();
        ~StaffRestRoom();
        bool canEnter(Person* p_person);
    private:
};
std::ostream& operator<<(std::ostream&, const StaffRestRoom&);
#endif
