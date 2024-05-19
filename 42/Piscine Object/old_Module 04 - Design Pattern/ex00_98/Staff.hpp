//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STAFF_HPP__
# define __STAFF_HPP__

#include <iostream>

#include "Person.hpp"

namespace SpringfieldElementary {

class Staff: public Person {
    public:
        using Person::Person; // Inherit constructors (it's the same constructor)
        virtual ~Staff();
        void sign();
    private:
};
std::ostream & operator<<(std::ostream &, const Staff &);
}
#endif
