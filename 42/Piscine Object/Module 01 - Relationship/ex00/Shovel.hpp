//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SHOVEL_HPP__
# define __SHOVEL_HPP__

#include <iostream>

#include "Tool.hpp"

namespace relationship {
    class Shovel: public Tool {
        public:
            Shovel();
            ~Shovel();
            void use();
        private:
    };
}
#endif
