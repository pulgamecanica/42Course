//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Contact.hpp"

Contact::Contact() {
	is_saved = false;
};
Contact::~Contact() {}

Contact::Contact(std::string fn, std::string ln, std::string nn, std::string pn, std::string ds) {
	first_name = fn;
	last_name = ln;
	nickname = nn;
	phone_number = pn;
	darkest_secret = ds;
	if (fn.empty() || ln.empty() || nn.empty() || pn.empty() || ds.empty())
		is_saved = false;
	else
		is_saved = true;
}

Contact Contact::operator= (Contact c)
{
	std::swap(first_name, c.first_name);
	std::swap(last_name, c.last_name);
	std::swap(nickname, c.nickname);
	std::swap(phone_number, c.phone_number);
	std::swap(darkest_secret, c.darkest_secret);
	return (*this);
}

void Contact::print_short() {
	std::cout << "|" << first_name << "|" << last_name << "|" << nickname << "|" << std::endl;
}

void Contact::print() {
	std::cout << "First Name: " << first_name << "\nLast Name: " << last_name << "\nNickname: " << nickname << "\nPhone Number: " << phone_number << "\nDarkest Secret: " << darkest_secret << std::endl;
}

bool Contact::saved()
{
	return (is_saved);
}
