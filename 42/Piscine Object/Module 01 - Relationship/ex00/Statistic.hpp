//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STATISTIC_HPP__
# define __STATISTIC_HPP__

#include <iostream>

namespace relationship {
    struct Statistic {
        Statistic();
        ~Statistic();
        int level;
        int exp;
    };
    std::ostream& operator<<(std::ostream&, const Statistic&);
}
#endif
