//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "hi_this_is_brian.hpp"

int	main(void)
{
	std::string str("HI THIS IS BRAIN");
	std::string *str_ptr;
	std::string &str_ref(str);

	str_ptr = &str;
	// str_ref = &str;
	std::cout << "Memory address: " << &str << "\nPTR Memory Address: " << &str_ptr << "\nREF Memory Address: " << &str_ref << std::endl;
	std::cout << "Value: " << str << "\nPtr Value: " << str_ptr << "\nRef Value: " << str_ref << std::endl;
	return (0);
}
