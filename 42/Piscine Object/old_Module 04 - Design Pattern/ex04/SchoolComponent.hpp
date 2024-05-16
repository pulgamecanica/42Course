//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SCHOOLCOMPONENT_HPP__
# define __SCHOOLCOMPONENT_HPP__

#include <iostream>

#include "HeadMaster.hpp"

class SchoolComponent {
    public:
        SchoolComponent(HeadMaster * hm = nullptr);
        virtual ~SchoolComponent() = 0;
    protected:
        HeadMaster * hm_;
};
#endif
