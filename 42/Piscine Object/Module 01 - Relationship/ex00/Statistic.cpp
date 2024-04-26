//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Statistic.hpp"

namespace relationship {
    Statistic::Statistic() {
    	(void)level;
    	(void)exp;
    }

    Statistic::~Statistic() {
        // std::cout << "Statistic" << " destroyed" << std::endl;
    }

    std::ostream& operator<<(std::ostream& s, const Statistic& stat) {
        s << "lvl:"<< stat.level << ", xp:" << stat.exp; 
        return (s);
    }
}
