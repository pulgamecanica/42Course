//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm> // for std::find

#include "ex03.inc"

#include "apprentice.hpp"

Apprentice::Apprentice(): ContractEmployee(22), school_hours_(3) {
    // For the contract of one year the following days are vacations
    // 4, 2, 42, 142, 242, 342
    no_work_days_.push_back(4);
    no_work_days_.push_back(2);
    no_work_days_.push_back(42);
    no_work_days_.push_back(142);
    no_work_days_.push_back(242);
    no_work_days_.push_back(342);
    // Students always get sick the 22th, 55th, 164th & 299th day, everyone knows this
    no_work_days_.push_back(22);
    no_work_days_.push_back(55);
    no_work_days_.push_back(164);
    no_work_days_.push_back(299);
    // Student birthday is the 118th day of the year, you can take the day off
    no_work_days_.push_back(118);
}

Apprentice::~Apprentice() {
    if (DEBUG)
        std::cout << RED << "Apprentice destroyed" << ENDC << std::endl;
}

double Apprentice::calculateSalary(int day) {
    if (std::find(no_work_days_.begin(), no_work_days_.end(), day) == no_work_days_.end())
        return ((7 - school_hours_) * hourlyValue) + (school_hours_ * (hourlyValue / 2));
    return 0;
}
