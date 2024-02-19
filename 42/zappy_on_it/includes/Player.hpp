//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PLAYER_HPP__
# define __PLAYER_HPP__

#include <iostream>

class Player {
	public:
		Player();
		Player(const Player&);
		~Player();
		Player&	operator= (const Player&); // const for safety... not super nesessary
};

std::ostream&	operator<<(std::ostream&, const Player&);

#endif

