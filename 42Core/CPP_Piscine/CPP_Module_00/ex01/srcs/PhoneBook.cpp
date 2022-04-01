//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "PhoneBook.hpp"

PhoneBook::PhoneBook() {
	list_size = 0;
}

PhoneBook::~PhoneBook() {}

PhoneBook PhoneBook::operator++ () {
	if (list_size >= PB_SIZE)
		return (*this);
	list_size++;
	return (*this);
}

PhoneBook PhoneBook::operator-- () {
	if (list_size <= 0)
		return (*this);
	list_size--;
        return (*this);
}

void PhoneBook::print() {
	std::cout << "|  INDEX   |FIRST NAME| LAST NAME | NICKNAME |" << std::endl;
	if (list_size <= 0)
	{
		std::cout << "Your PhoneBook is empty... :(" <<  std::endl;
		return ;
	}
	for (int i = 0; i < list_size; i++)
	{
		std::cout << "|    " << i + 1 << "     ";
		list[i].print_short();
	}
}

void PhoneBook::add_contact(Contact c) {
	if (!c.saved())
	{
		std::cout << "Sorry Can't create Contact make sure all fields are filled" << std::endl;
		return ;
	}
	list[list_size % PB_SIZE] = c;
	++*this;
}
