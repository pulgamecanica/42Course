//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm> // for std::find

#include "ex03.inc"

#include "contractEmployee.hpp"

ContractEmployee::ContractEmployee(int rate): Employee("contract employee", rate) {
	// For the contract of one year the following days are vacations
    // 4, 2, 42, 142, 242, 342
    no_work_days_.push_back(4);
    no_work_days_.push_back(2);
    no_work_days_.push_back(42);
    no_work_days_.push_back(142);
    no_work_days_.push_back(242);
    no_work_days_.push_back(342);
    // Workers always get sick the 66th day, everyone knows this
    no_work_days_.push_back(66);
    // Workers birthday is the 122th day of the year, you can take the day off    
    no_work_days_.push_back(122);
}

ContractEmployee::~ContractEmployee() {
    if (DEBUG)
        std::cout << RED << "ContractEmployee destroyed" << ENDC << std::endl;
}

double ContractEmployee::calculateSalary(int day) {
    if (std::find(no_work_days_.begin(), no_work_days_.end(), day) == no_work_days_.end())
        return hourlyValue * 7;
    return 0;
}
