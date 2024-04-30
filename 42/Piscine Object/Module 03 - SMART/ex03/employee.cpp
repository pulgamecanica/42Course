//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex03.inc"

#include "employee.hpp"

Employee::Employee(const std::string& type, int hourlyValue):
    type_(type), hourlyValue(hourlyValue), last_day_worked_(0) {
    ;
}

Employee::~Employee() {
    if (DEBUG)
        std::cout << RED << "Employee " << ENDC << type_ << RED << " destroyed" << ENDC << std::endl;
}

double Employee::calculateSalaryBetween(int start, int end) {
    double total = 0;
    for (int i = start; i < end; ++i) {
        total += calculateSalary(i);
        if (DEBUG)
            std::cout << BLUE << *this << ENDC << " day " << i << GREEN << " +$" << calculateSalary(i) << ENDC << std::endl;
    }
    return total;
}

int Employee::executeWorkday() {
    return last_day_worked_++;
}
std::ostream& operator<<(std::ostream& s, const Employee& param) {
    s << param.type_;
    return (s);
}

