//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __EMPLOYEEMANAGEMENT_HPP__
# define __EMPLOYEEMANAGEMENT_HPP__

#include <iostream>
#include <vector>

#include "employee.hpp"


class EmployeeManagement {
    public:
        enum WorkDays {
            MONDAY,
            TUESDAY,
            WEDNESDAY,
            THURSDAY,
            FRIDAY
        };
        EmployeeManagement();
        ~EmployeeManagement();
        void addEmployee(Employee *);
        void removeEmployee(Employee *);
        void executeWorkday(); // 7hrs of work!
        void calculatePayroll();
    private:
        std::vector<Employee *> employees_;
        int current_work_day_;
};
#endif
