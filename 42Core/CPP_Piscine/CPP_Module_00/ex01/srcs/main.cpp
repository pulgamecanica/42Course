//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "my_awesome_phone_book.hpp"

void menu(){
	std::cout << C_GREEN << "	ADD" << C_END << "	: add a new contact to your phone book (max. 8)" << std::endl;
	std::cout << C_GREEN << "	SEARCH" << C_END << "	: will display the Phone Book & you can choose a contact to inspect" << std::endl;
	std::cout << C_GREEN << "	EXIT" << C_END << "	: Exit the program :)" << std::endl;
}

int check_line(std::string line)
{
	if (line.compare("ADD") != 0 && line.compare("SEARCH") != 0 && line.compare("EXIT") != 0)
		return (0);
	return (1);
}

Contact create_contact(void)
{
	std::string lines[5];

	std::cout << C_GREEN << "Creating a new contact" << C_END << std::endl;
	std::cout << "1- First Name | 2- Last Name | 3- Nickname | 4-Phone Number | 5- Darkest Secret " << std::endl;
	for (int i = 0; i < 5; i++)
	{
		std::cout << i + 1<< "- ";
		std::cin >> lines[i];
		if (lines[i].empty())
			break ;
	}
	Contact c(lines[0], lines[1], lines[2], lines[3], lines[4]);
	return (c);
}

int main(void)
{
	PhoneBook	pb;
	std::string		line;
	std::cout << C_RED << "***PhoneBook Menu***" << C_END << std::endl;
	while (true)
	{
		menu();
		std::cin >> line;
		if (check_line(line))
		{
			if (line.compare("ADD"))
				pb.add_contact(create_contact());
			else if (line.compare("SEARCH"))
				pb.print();
			else
				break ;
			continue ;
		}
		std::cout << "Invalid command <" << line << ">" << std::endl;
	}
	std::cout << "Bye Bye, Phone Book deleted ..." << std::endl;
	return (0);
}
