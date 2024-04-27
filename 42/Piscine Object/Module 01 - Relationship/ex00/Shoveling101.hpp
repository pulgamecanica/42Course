//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SHOVELING101_HPP__
# define __SHOVELING101_HPP__

#include <iostream>
#include "WorkShop.hpp"

namespace relationship {
    class Shoveling101: public WorkShop {
        public:
            Shoveling101();
            ~Shoveling101();
            bool hasValidTool(Worker *w);
        private:
    };
}
#endif
