//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Harl.hpp"

void Harl::debug(void) {std::cout << "I love having extra bacon for my 7XL-double-cheese-triple-pickle-specialketchup burger. I really do!";}

void Harl::info(void) {std::cout << "I cannot believe adding extra bacon costs more money. You didn’t put enough bacon in my burger! If you did, I wouldn’t be asking for more!";}

void Harl::warning(void) {std::cout << "I think I deserve to have some extra bacon for free. I’ve been coming for years whereas you started working here since last month.";}

void Harl::error(void) {std::cout << "This is unacceptable! I want to speak to the manager now.";}

void Harl::complain( std::string level )  {
	std::string array[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
	void (*funcs[])(void) = {debug, info, warning, error};
	
	for (int i = 0; i < 4; i++)
		if (array[i] == level)
			funcs[i]();
	std::cout << std::endl;
}