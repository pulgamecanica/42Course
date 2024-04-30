//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __APPRENTICE_HPP__
# define __APPRENTICE_HPP__

#include <iostream>

#include "contractEmployee.hpp"

class Apprentice: public ContractEmployee {
    public:
        Apprentice();
        ~Apprentice();
        double calculateSalary(int day);
        const int school_hours_;
};
#endif
