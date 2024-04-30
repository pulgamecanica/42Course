//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex03.inc"

#include "tempWorker.hpp"

TempWorker::TempWorker(): Employee("temp worker", 15) {

}

TempWorker::~TempWorker() {
    if (DEBUG)
        std::cout << RED << "TempWorker destroyed" << ENDC << std::endl;
}

double TempWorker::calculateSalary(int day) {
    int hours_worked = 0;
    try {
        hours_worked = hours_worked_map_[day];
    } catch (std::exception & e) {
        ;
    }
    return hours_worked * hourlyValue;
}
        
int TempWorker::executeWorkday() {
    if (last_day_worked_ % 30 == 0) {
        if (DEBUG)
            std::cout << RED << "NO WORK\t" << ENDC << "Today I can't work" << std::endl;
        last_day_worked_++;
        return -1;
    }
    hours_worked_map_[last_day_worked_] = 4.5;
    if (DEBUG)
        std::cout << GREEN << "WORK\t" << YELLOW << "day: " << last_day_worked_ << BLUE << " " << *this << ENDC << " Worked for 4.5hrs" << std::endl;
    last_day_worked_++;
    return 1;
}
