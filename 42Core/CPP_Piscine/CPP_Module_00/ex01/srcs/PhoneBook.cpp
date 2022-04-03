//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "PhoneBook.hpp"

PhoneBook::PhoneBook() {list_size = 0;}

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
	std::cout << "**********************************************" << std::endl;
	std::cout << "|  INDEX   |FIRST_NAME| LASTNAME | NICKNAME |" << std::endl;
	std::cout << "|---------- ---------- ----------- ---------|" << std::endl;
	if (list_size <= 0)
		std::cout << "|Your PhoneBook is empty... :(               |" <<  std::endl;
	for (int i = 0; i < list_size; i++)
	{
		std::cout << "|";
		std::cout.width(10); std::cout << std::right << i + 1;
		list[i].print_short();
	}
	std::cout << "**********************************************" << std::endl;
}

void PhoneBook::add_contact(Contact c) {
	if (c.saved() == false)
	{
		std::cout << "Sorry Can't create Contact make sure all fields are filled" << std::endl;
		return ;
	}
	list[list_size % PB_SIZE] = c;
	++*this;
	std::cout << "Succesfully added Contact!" << std::endl;
}

void PhoneBook::print_contact(int index) {
	if (index <= 0 || index > list_size)
		std::cout << "Sorry that's not a valid Contact Index... :(" << std::endl;
	else
		list[index - 1].print();
}
