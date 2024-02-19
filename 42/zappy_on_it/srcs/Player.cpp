//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Player.hpp"

Player::Player() {
	// TODO (default constructor)
}

Player::Player(const Player& param) {
	// TODO (copy constructor)
	(void)param;
}

Player::~Player() {
	std::cout << "Player" << " destroyed" << std::endl;
	// TODO (destructor)
}

Player& Player::operator= (const Player& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Player& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

