//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Shoveling101.hpp"
#include "Shovel.hpp"

namespace relationship {
    Shoveling101::Shoveling101(): WorkShop("Shoveling 101") {
    }

    Shoveling101::~Shoveling101() {
    }

    bool Shoveling101::hasValidTool(Worker *w) {
        return (w->getTool<Shovel>() != NULL);
    }
}
