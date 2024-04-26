//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Position.hpp"

namespace relationship {
    Position::Position(int x, int y, int z) {
    	this->x = x;
    	this->y = y;
    	this->z = z;
    }

    Position::~Position() {
        // std::cout << "Position" << " destroyed" << std::endl;
    }

    std::ostream& operator<<(std::ostream& s, const Position& p) {
        s << p.x << ", " << p.y << ", " << p.z;
        return (s);
    }
}
