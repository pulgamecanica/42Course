//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Statistic.hpp"

namespace relationship {
    Statistic::Statistic(): level(0), exp(0) {
    	(void)level;
    	(void)exp;
    }

    Statistic::~Statistic() {
    }

    std::ostream& operator<<(std::ostream& s, const Statistic& stat) {
        s << "lvl:"<< stat.level << ", xp:" << stat.exp; 
        return (s);
    }
}
