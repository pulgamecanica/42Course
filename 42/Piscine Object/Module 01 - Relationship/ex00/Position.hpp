//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __POSITION_HPP__
# define __POSITION_HPP__

#include <iostream>

namespace relationship {
    struct Position {
        Position(int x = 0, int y = 0, int z = 0);
        ~Position();
        int x;
        int y;
        int z;
    };
    std::ostream& operator<<(std::ostream&, const Position&);
}
#endif
