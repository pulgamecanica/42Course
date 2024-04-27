//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __WOODWORKING_HPP__
# define __WOODWORKING_HPP__

#include <iostream>

#include "WorkShop.hpp"

namespace relationship {
    class WoodWorking: public WorkShop {
        public:
            WoodWorking();
            ~WoodWorking();
            bool hasValidTool(Worker *w);
        private:
    };
}
#endif
