//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __TEMPWORKER_HPP__
# define __TEMPWORKER_HPP__

#include <iostream>
#include <map>

#include "employee.hpp"

// Temp Worker doesn't work the first day of everymonth
// It must go visit his best friend forever
class TempWorker: public Employee {
    public:
        TempWorker();
        ~TempWorker();
        double calculateSalary(int day) ;
        int executeWorkday();
    private:
        std::map<int, int> hours_worked_map_; // For the day, how many hours he worked
};
#endif
