//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CONTRACTEMPLOYEE_HPP__
# define __CONTRACTEMPLOYEE_HPP__

#include <iostream>
#include <vector>

#include "employee.hpp"

class ContractEmployee: public Employee {
    public:
        ContractEmployee(int rate = 42);
        virtual ~ContractEmployee();
        virtual double calculateSalary(int day);
    protected:
        std::vector<int> no_work_days_;
};
#endif
