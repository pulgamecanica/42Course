//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "car.hpp"

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;

    
    std::cout << "- - - 1  - - -" << std::endl;;
    ex00::car c;
    std::cout << c << std::endl;
    
    std::cout << "- - - 2  - - -" << std::endl;;
    c.start();
    std::cout << c << std::endl;
    
    std::cout << "- - - 3  - - -" << std::endl;;
    c.shift_gears_up();
    c.shift_gears_up();
    c.accelerate(4.2);
    std::cout << c << std::endl;
    
    std::cout << "- - - 4  - - -" << std::endl;;
    c.accelerate(2.1);
    c.shift_gears_up();
    std::cout << c << std::endl;
    
    std::cout << "- - - 5  - - -" << std::endl;;
    c.apply_force_on_brakes(5.0);
    c.shift_gears_down();
    std::cout << c << std::endl;
    
    std::cout << "- - - 6  - - -" << std::endl;;    
    c.turn_wheel(15);
    c.accelerate(2.1);
    c.shift_gears_up();
    std::cout << c << std::endl;
    
    std::cout << "- - - 7  - - -" << std::endl;;
    c.straighten_wheels();
    c.apply_force_on_brakes(2.1);
    c.shift_gears_down();
    std::cout << c << std::endl;
    
    std::cout << "- - - 8  - - -" << std::endl;;
    c.turn_wheel(-15);
    c.apply_emergency_brakes();
    std::cout << c << std::endl;
    
    
    std::cout << "- - - 9  - - -" << std::endl;;
    c.stop();
    std::cout << c << std::endl;
    return (0);
}
