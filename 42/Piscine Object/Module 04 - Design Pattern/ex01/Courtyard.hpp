//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COURTYARD_HPP__
# define __COURTYARD_HPP__

#include <iostream>

#include "Room.hpp"

class Courtyard: public Room {
    public:
        Courtyard();
        ~Courtyard();
    private:
};
std::ostream& operator<<(std::ostream&, const Courtyard&);
#endif
