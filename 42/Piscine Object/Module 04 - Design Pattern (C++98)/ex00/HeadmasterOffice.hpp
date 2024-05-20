//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HEADMASTEROFFICE_HPP__
# define __HEADMASTEROFFICE_HPP__

#include <iostream>

#include "Room.hpp"

class HeadmasterOffice: public Room {
    public:
        HeadmasterOffice();
        ~HeadmasterOffice();
        bool canEnter(Person* p_person);
    private:
};
std::ostream& operator<<(std::ostream&, const HeadmasterOffice&);
#endif
