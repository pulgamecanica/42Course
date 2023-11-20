//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Menu.hpp"

Menu::Menu() {
	// TODO (default constructor)
}

Menu::~Menu() {
	std::cout << "Menu" << " destroyed" << std::endl;
}

void Menu::drawContainer() const {

}

// Menu::Menu(const Menu& param) {
// 	// TODO (copy constructor)
// 	(void)param;
// }

// Menu& Menu::operator= (const Menu& param) {
// 	// TODO (Assignment operatior)
// 	// std::swap()
// 	(void)param;
// 	return (*this);
// }

std::ostream& operator<<(std::ostream& s, const Menu& param) {
	(void)param;
	s << "Menu";
	return (s);
}

