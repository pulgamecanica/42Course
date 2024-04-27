//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __DERIVED_HPP__
# define __DERIVED_HPP__

#include <iostream>

#include "Base.hpp"

class Derived: public Base {
    public:
        Derived(const std::string & name);
        ~Derived();
    private:
        std::string name_;
};
#endif
