//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HAMMER_HPP__
# define __HAMMER_HPP__

#include <iostream>

#include "Tool.hpp"

namespace relationship {
    class Hammer: public Tool {
        public:
            Hammer();
            ~Hammer();
            void use();
        private:
    };
}
#endif
