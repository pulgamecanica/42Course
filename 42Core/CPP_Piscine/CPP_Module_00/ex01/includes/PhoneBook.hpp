//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PHONEBOOK_HPP__
# define __PHONEBOOK_HPP__

#include "Contact.hpp"
#include <cstring>
#include <iostream>

class	Contact;

#define PB_SIZE 8

class PhoneBook {
	Contact	list[8];
	short int list_size;
	PhoneBook       operator++ ();
	PhoneBook       operator-- ();
	public:
		PhoneBook();
		~PhoneBook();
		void		print();
		void		add_contact(Contact);
		void		print_contact(int);
};

#endif

