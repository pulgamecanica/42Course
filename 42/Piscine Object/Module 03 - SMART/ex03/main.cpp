//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex03.inc"
#include "employeeManagement.hpp"
#include "tempWorker.hpp"
#include "contractEmployee.hpp"
#include "apprentice.hpp"

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    EmployeeManagement em;

    std::cout << YELLOW << "- - - - TempWorker Test - - - -" << ENDC << std::endl;
    TempWorker tw1;

    em.addEmployee(&tw1);
    for (int i = 0; i < 80; ++i)
    {
        em.executeWorkday();
    }
    em.calculatePayroll();
    em.removeEmployee(&tw1);
    std::cout << YELLOW << "- - - - ContractEmployee Test - - - -" << ENDC << std::endl;
    ContractEmployee ce1;

    em.addEmployee(&ce1);
    for (int i = 0; i < 35; ++i)
    {
        em.executeWorkday();
    }
    em.calculatePayroll();
    em.removeEmployee(&ce1);
    std::cout << YELLOW << "- - - - apprentice Test - - - -" << ENDC << std::endl;
    Apprentice ap1;

    em.addEmployee(&ap1);
    for (int i = 0; i < 30; ++i)
    {
        em.executeWorkday();
    }
    em.calculatePayroll();
    em.removeEmployee(&ap1);
    std::cout << YELLOW << "- - - - - - - - - - - - - - - -" << std::endl;
    return (0);
}
