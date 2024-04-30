//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __EMPLOYEE_HPP__
# define __EMPLOYEE_HPP__

#include <iostream>

class Employee {
    public:
        Employee(const std::string& type, int hourlyValue);
        virtual ~Employee();
        double calculateSalaryBetween(int start, int end);
        virtual double calculateSalary(int day) = 0;
        virtual int executeWorkday();
        const std::string type_;
    protected:
        int hourlyValue;
        int last_day_worked_;
};
std::ostream& operator<<(std::ostream& s, const Employee& param);
#endif
