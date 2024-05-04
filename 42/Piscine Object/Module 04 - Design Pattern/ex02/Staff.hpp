//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STAFF_HPP__
# define __STAFF_HPP__

#include <iostream>

#include "Person.hpp"
#include "Form.hpp"

class Staff: public Person {
    public:
        Staff(const std::string & name);
        virtual ~Staff();
        void sign(Form* p_form);
    private:
};
std::ostream& operator<<(std::ostream&, const Staff&);
#endif
