//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm> // for std::find
#include "ex03.inc"
#include "employeeManagement.hpp"

EmployeeManagement::EmployeeManagement(): current_work_day_(0) {
	(void)employees_;
}

EmployeeManagement::~EmployeeManagement() {
    if (DEBUG)
        std::cout << RED << "EmployeeManagement destroyed" << ENDC << std::endl;
}

void EmployeeManagement::addEmployee(Employee *e) {
    employees_.push_back(e);
    if (DEBUG)
        std::cout << GREEN << "ADD\t" << BLUE << *e << ENDC << std::endl;
}

void EmployeeManagement::removeEmployee(Employee *e) {
    std::vector<Employee *>::iterator it = std::find(employees_.begin(), employees_.end(), e);
    if (it != employees_.end())
        employees_.erase(it);
    if (DEBUG)
        std::cout << RED << "REMOVE\t" << BLUE << *e << ENDC << std::endl;
}

void EmployeeManagement::executeWorkday() {
    for (std::vector<Employee *>::iterator i = employees_.begin(); i != employees_.end(); ++i)
        (*i)->executeWorkday();
    current_work_day_++;
}

void EmployeeManagement::calculatePayroll() {
    if (current_work_day_ < 30) {
        std::cout << RED << "Warning, cannot caluclate Payroll because there is not enough data, at least work for 1 month" << std::endl;
        return ;
    }
    int latest_compleated_month = current_work_day_ / 30;
    int first_day_of_month = (latest_compleated_month - 1) * 30;
    double total = 0;
    std::cout << BLUE << "Payroll for month: " << YELLOW << latest_compleated_month << ENDC << std::endl;
    for (std::vector<Employee *>::iterator i = employees_.begin(); i != employees_.end(); ++i) {
        double salary = (*i)->calculateSalaryBetween(first_day_of_month, first_day_of_month + 30);
        std::cout << "  Employee: " << BLUE << **i << ENDC << " Gets: $" << salary << std::endl;
        total += salary;
    }
    std::cout << BLUE "Total Payroll " << GREEN << "$" << total << ENDC << std::endl;
}

