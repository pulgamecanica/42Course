//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CONTACT_HPP__
# define __CONTACT_HPP__

#include <string>
#include <iostream>

class Contact {
	std::string	first_name;
	std::string	last_name;
	std::string	nickname;
	std::string	phone_number;
	std::string	darkest_secret;
	bool		is_saved;
	public:
		Contact();
		Contact(std::string, std::string, std::string, std::string, std::string);
		~Contact();
		Contact	operator= (Contact);
		void	print_short();
		void	print();
		bool	saved();
};

#endif

